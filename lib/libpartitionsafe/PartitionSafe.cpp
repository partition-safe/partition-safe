//
// Created by marc on 15-11-16.
//

#include "PartitionSafe.h"

void PartitionSafe::create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size, const char *username, const char *password) {
    // Try to create the vault
    Vault::create(label, size, vaultPath);
    Vault *vault = Vault::init(vaultPath);

    // Try to create the key store
    KeyStore::create(keyStorePath);
    KeyStore *keyStore = KeyStore::init(keyStorePath);

    // Save some metadata
    keyStore->setMetadata("uuid", vault->header->UUID);
    keyStore->setMetadata("label", vault->header->label);

    // Create the user and save it
    User *user = User::create(username, password);
    keyStore->saveUser(user);
    keyStore->getUser(username, &user);

    // Create the first encryption key
    Key *key = Key::create(user, password, 0);
    keyStore->saveKey(key);
    keyStore->getKey(0, user, &key);
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
    User *user;
    keyStore->getUser(username, &user);

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
