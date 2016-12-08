//
// Created by marc on 15-11-16.
//

#include "PartitionSafe.h"

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

    // Try to create the vault
    Vault::create(label, size, vaultPath, (const unsigned char *)encrypted);
    Vault *vault = Vault::init(vaultPath);

    // Save some metadata
    keyStore->setMetadata("uuid", vault->header->UUID);
    keyStore->setMetadata("label", vault->header->label);
    keyStore->setMetadata("encrypted_identifier", (const char *)encrypted);

    // Cleanup
    delete saltedPassword;
    delete encryptionKey;
    delete encrypted;
    delete user;
    delete key;
    delete vault;
    delete keyStore;
}

PartitionSafe *PartitionSafe::init(const char *vaultPath, const char *keyStorePath, const char *username, const char *password) {
    // Get the vault instance
    vault = Vault::init(vaultPath);

    // Get the key store instance
    keyStore = KeyStore::init(keyStorePath);

    // Check header stuff
    char *uuid;
    keyStore->getMetadata("uuid", &uuid);
    if(strcmp(vault->header->UUID, uuid) != 0) throw "Vault and keystore aren't a couple";

    // Retrieve user
    keyStore->getUser(username, &user);

    // Retrieve root key
    keyStore->getKey(0, user, &key);

    // Get the decrypted key
    unsigned char *decryptedKey;
    key->decrypt(user, password, vault->header->identifier_encrypted, &decryptedKey);

    // Check the decrypted key
    std::cout << "Decrypted identifier: " << decryptedKey << std::endl;

    // Cleanup
    delete decryptedKey;

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
        // Create the salted password
        char *saltedPassword;
        (*user)->saltedPassword(password, (*user)->salt, &saltedPassword);

        // Encrypt key
        unsigned char *encryptionKey;
        this->key->decrypt(saltedPassword, this->key->key, &encryptionKey);
        *key = Key::create(*user, password, encryptionKey, 0);

        // Free memory
        delete[] saltedPassword;
    } else {
        // Create a new key
        *key = Key::create(*user, password);
    }

    // Create the new root key based on the current encryption key
    keyStore->saveKey(*key);
    keyStore->getKey(0, *user, key);
}
