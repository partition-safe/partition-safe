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
     * Destructor of the partition safe instance.
     */
    ~PartitionSafe();

    /**
     * Get the vault instance.
     *
     * @return The vault instance
     */
    Vault *getVault();

    /**
     * Get the key store instance.
     *
     * @return The key store instance
     */
    KeyStore *getKeyStore();

    /**
     * Get the current user.
     *
     * @return The current user
     */
    User *getUser();

    /**
     * Get the current root key.
     *
     * @return The root key
     */
    Key *getKey();

    /**
     * Create a new PartitionSafe instance.
     *
     * @param vaultPath The path to place the new vault
     * @param keyStorePath The path to place the new key store
     * @param label The label of the partition (max 40 characters)
     * @param size The size of the partition in bytes
     * @param username The username of the first user
     * @param password The password of the first user
     */
    void create(const char *vaultPath, const char *keyStorePath, const char label[40], const unsigned size,
                const char *username, const char *password);

    /**
     * Initialize a PartitionSafe instance.
     *
     * @param vaultPath The path to the vault file
     * @param keyStorePath The path to the key store file
     * @param username The username of the user to sign in
     * @param password The password of the user to sign in
     *
     * @return The initialized PartitionSafe instance
     */
    PartitionSafe *init(const char *vaultPath, const char *keyStorePath, const char *username, const char *password);

    /**
     * Open the partition safe instance.
     *
     * @return The opened PartitionSafe instance
     */
    PartitionSafe *open();

    /**
     * Create a new user and it's root key.
     *
     * The user will be saved to the current key store.
     *
     * @param username The username of the new user
     * @param password The password of the new user
     * @param user OUTPUT The created user object
     * @param key OUTPUT The created root key object
     * @param keyStore The key store instance to use
     */
    void createUser(const char *username, const char *password, User **user, Key **key, KeyStore *keyStore = nullptr);

};


#endif //PARTITIONSAFE_PARTITIONSAFE_H
