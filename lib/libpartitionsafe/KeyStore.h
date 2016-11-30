//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>
#include "../libsqlite/sqlite3.h"
#include "database/User.h"
#include "../libmbedtls/include/mbedtls/rsa.h"

#define KEY_SIZE 2048
#define EXPONENT 65537

class KeyStore {
    /**
     * File path to this partition
     */
    const char* path;

    /**
     * The sqlite handle.
     */
    sqlite3 *sqliteHandle;

public:
    /**
     * Constructor of a key store.
     *
     * @param path
     * @return
     */
    KeyStore(const char* path);

    /**
     * Create a new key store.
     *
     * @param path The path to place the new key store
     *
     * @return
     */
    static KeyStore *create(const char* path);

    /**
     * Open a key store instance.
     *
     * @param keyStorePath The path to the key store file
     *
     * @return The opened vault instance
     */
    static KeyStore *init(const char* keyStorePath);

    /**
     * Close the current key store instance.
     */
    void close();

    /**
     * Create a new user and add it to the database.
     *
     * @param username
     * @return
     */
    User *createUser(const char* username);

private:
    /**
     * Create metadata table statement.
     */
    static const char *STMT_CREATE_TABLE_METADATA;

    /**
     * Create users table statement.
     */
    static const char *STMT_CREATE_TABLE_USERS;

    /**
     * Create keys table statement.
     */
    static const char *STMT_CREATE_TABLE_KEYS;

    /**
     * Create notifications table statement.
     */
    static const char *STMT_CREATE_TABLE_NOTIFICATIONS;

    /**
     * Create a new key pair.
     *
     * @param pubKey
     * @param privKey
     */
    static mbedtls_rsa_context createKeyPair(unsigned char* *pubKey, unsigned char* *privKey);

};


#endif //PARTITIONSAFE_KEYSTORE_H
