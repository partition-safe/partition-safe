//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITIONSAFE_H
#define PARTITIONSAFE_PARTITIONSAFE_H


#include "Vault.h"
#include "KeyStore.h"

class PartitionSafe {
    /**
     * The current vault instance.
     */
    Vault *vault;

    /**
     * Our key storage.
     */
    KeyStore *keyStore;

    /**
     * The current user.
     */
    User *user;

    /**
     * The current root key.
     */
    Key *key;

public:
    /**
     * Get the vault instance.
     * @return
     */
    Vault *getVault();

    /**
     * Get the key store instance.
     * @return
     */
    KeyStore *getKeyStore();

    /**
     * Get the current user.
     * @return
     */
    User *getUser();

    /**
     * Get the current root key.
     * @return
     */
    Key *getKey();

    /**
     * Create a new vault with key store.
     *
     * @param vaultPath The path to place the new vault
     * @param keyStorePath The path to place the new key store
     * @param label The label of the partition
     * @param size The size of the partition in bytes
     */
    void create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size, const char *username, const char *password);

    /**
     * Initialize a vault instance.
     *
     * @param vaultPath The path to the vault file
     * @param keyStorePath The path to the key store file
     *
     * @return
     */
    PartitionSafe* init(const char* vaultPath, const char* keyStorePath, const char *username, const char *password);

    /**
     * Open the partition safe instance.
     *
     * @return
     */
    PartitionSafe* open();

    /**
     * Create a new user and it's root key.
     *
     * The user will be saved to the current key store.
     *
     * @param username
     * @param password
     * @param user
     */
    void createUser(const char *username, const char *password, User **user, Key **key, KeyStore *keyStore = nullptr);

};


#endif //PARTITIONSAFE_PARTITIONSAFE_H
