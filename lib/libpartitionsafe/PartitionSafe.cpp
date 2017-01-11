//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "PartitionSafe.h"
#include "NotificationCentre.h"
#include "../libfatfs/src/diskio.h"

PartitionSafe::~PartitionSafe() {
    delete vault;
    delete keyStore;
    delete user;
    delete key;
}

void PartitionSafe::create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size, const char *username, const char *password) {
    // Try to create the key store
    KeyStore::create(keyStorePath);
    KeyStore *keyStore = KeyStore::init(keyStorePath);

    // Create the user and save it
    User *user; Key *key;
    createUser(username, password, &user, &key, keyStore);

    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Decrypt key
    unsigned char *encryptionKey;
    key->decrypt(saltedPassword, key->key, &encryptionKey);

    // Encrypt the test string
    unsigned char *encrypted;
    key->encrypt((const char *)encryptionKey, Partition::IDENTIFIER, &encrypted);

    // Setup the encryption config
    std::fill_n(_disk_encryption_conf.key, 32, 0x00);
    std::fill_n(_disk_encryption_conf.iv, 16, 0x00);
    memcpy(_disk_encryption_conf.key, encryptionKey, 32);
    memcpy(_disk_encryption_conf.iv, encryptionKey, 16);

    // Try to create the vault
    Vault::create(label, size, vaultPath, (const unsigned char *)encrypted);
    Vault *vault = Vault::init(vaultPath);

    // Save some metadata
    keyStore->setMetadata("uuid", vault->header->UUID);
    keyStore->setMetadata("label", vault->header->label);
    keyStore->setMetadata("encrypted_identifier", (const char *)encrypted);

#ifndef __WIN32
    // Cleanup
    delete[] saltedPassword;
    delete[] encryptionKey;
    delete[] encrypted;
//    delete user;
//    delete key;
    delete vault;
    delete keyStore;
#endif
}

PartitionSafe *PartitionSafe::init(const char *vaultPath, const char *keyStorePath, const char *username, const char *password) {
    // Get the vault instance
    vault = Vault::init(vaultPath);

    // Get the key store instance
    keyStore = KeyStore::init(keyStorePath);

    // Initialize the notification centre
    NotificationCentre::getInstance(this);

    // Check header stuff
    char *uuid = new char[36]();
    keyStore->getMetadata("uuid", &uuid);
    if(strcmp(vault->header->UUID, uuid) != 0) throw "Vault and keystore aren't a couple";

    // Retrieve user
    keyStore->getUser(username, &user);

    // Retrieve root key
    keyStore->getKey(0, user, &key);

    // Decrypt key
    unsigned char *decryptionKey;
    key->decrypt(user, password, key->key, &decryptionKey);

    // Get the decrypted key
    unsigned char *decryptedIdentifier;
    key->decrypt((const char *)decryptionKey, vault->header->identifier_encrypted, &decryptedIdentifier);

    // Check identifier
    if(strncmp((const char *)Partition::IDENTIFIER, (const char *)decryptedIdentifier, 14) != 0) {
        throw "Could not decrypt the identifier";
    }

    // Setup the encryption config
    std::fill_n(_disk_encryption_conf.key, 32, 0x00);
    std::fill_n(_disk_encryption_conf.iv, 16, 0x00);
    memcpy(_disk_encryption_conf.key, decryptionKey, 32);
    memcpy(_disk_encryption_conf.iv, decryptionKey, 16);

    // Cleanup
    delete[] decryptedIdentifier;

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::open() {
    // Open the vault
    vault->open();

    // Return myself
    return this;
}

Vault *PartitionSafe::getVault() {
    return vault;
}

KeyStore *PartitionSafe::getKeyStore() {
    return keyStore;
}

User *PartitionSafe::getUser() {
    return user;
}

Key *PartitionSafe::getKey() {
    return key;
}

void PartitionSafe::createUser(const char *username, const char *password, User **user, Key **key, KeyStore *keyStore) {
    // Keystore a nullptr?
    if(keyStore == nullptr) keyStore = this->keyStore;

    // Create the new user
    *user = User::create(username, password);
    keyStore->saveUser(*user);
    keyStore->getUser(username, user);

    // First decrypt the current encryption key and then create the key
    if(this->key) {
        // Create the new key
        *key = Key::create(*user, password, _disk_encryption_conf.key, 0);
    } else {
        // Create a new key
        *key = Key::create(*user, password);
    }

    // Create the new root key based on the current encryption key
    keyStore->saveKey(*key);
    keyStore->getKey(0, *user, key);
}
