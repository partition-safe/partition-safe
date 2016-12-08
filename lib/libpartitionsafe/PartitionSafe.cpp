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
//    char* saltedPassword = (char*)calloc(strlen(password) + strlen(user->salt) + 1, sizeof(char));
//    if (saltedPassword == NULL) throw "Could not allocate something";
//    char *saltedPassword = new char[strlen(password) + strlen(user->salt)]();
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Decrypt key
//    unsigned char *encryptionKey;
//    unsigned char *encryptionKey = (unsigned char *) calloc(ENCRYPTION_KEY_LENGTH + 1, sizeof(unsigned char));
//    if (*encryptionKey == NULL) throw "Could not allocate something";
    unsigned char *encryptionKey = new unsigned char[ENCRYPTION_KEY_LENGTH]();
    key->decrypt(saltedPassword, key->key, &encryptionKey);

    // Encrypt the test string
//    unsigned char *encrypted;
//    unsigned char *encrypted = (unsigned char *) calloc(ENCRYPTION_KEY_LENGTH + 1, sizeof(unsigned char));
//    if (*encrypted == NULL) throw "Could not allocate something";
    unsigned char *encrypted = new unsigned char[ENCRYPTION_KEY_LENGTH]();
    key->encrypt((const char *)encryptionKey, Partition::IDENTIFIER, &encrypted);

    // Try to create the vault
    Vault::create(label, size, vaultPath, (const unsigned char *)encrypted);
    Vault *vault = Vault::init(vaultPath);

    // Save some metadata
    keyStore->setMetadata("uuid", vault->header->UUID);
    keyStore->setMetadata("label", vault->header->label);
    keyStore->setMetadata("encrypted_identifier", (const char *)encrypted);
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
//        char* saltedPassword = (char*)calloc(strlen(password) + strlen((*user)->salt) + 1, sizeof(char));
//        if (saltedPassword == NULL) throw "Could not allocate something";
//        char* saltedPassword = new char[strlen(password) + strlen((*user)->salt) + 1]();
        (*user)->saltedPassword(password, (*user)->salt, &saltedPassword);

        // Encrypt key
//        unsigned char *encryptionKey;
//        unsigned char *encryptionKey = (unsigned char *) calloc(ENCRYPTION_KEY_LENGTH + 1, sizeof(unsigned char));
//        if (encryptionKey == NULL) throw "Could not allocate something";
        unsigned char* encryptionKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
        this->key->decrypt(saltedPassword, this->key->key, &encryptionKey);
        *key = Key::create(*user, password, encryptionKey, 0);

        // Free memory
//        delete[] saltedPassword;
//        delete[] encryptionKey;
    } else {
        // Create a new key
        *key = Key::create(*user, password);
    }

    // Create the new root key based on the current encryption key
    keyStore->saveKey(*key);
    keyStore->getKey(0, *user, key);
}
