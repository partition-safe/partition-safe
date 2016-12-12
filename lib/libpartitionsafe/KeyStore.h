//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>
#include "../libsqlite/sqlite3.h"
#include "database/User.h"
#include "database/Key.h"

class KeyStore {
    /**
     * File path to this partition
     */
    const char* path;

    /**
     * The sqlite handle.
     */
    sqlite3 *sqliteHandle;

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
     * Execute a statement with error handling.
     *
     * @param stmt The statement
     */
    static void execute(sqlite3 *db, const char *query);

public:
    /**
     * Constructor of a key store.
     *
     * @param path
     * @return
     */
    KeyStore(const char* path);

    /**
     * Keystore destructor.
     */
    ~KeyStore();

    /**
     * Create a new key store.
     *
     * @param path The path to place the new key store
     *
     * @return
     */
    static void create(const char* path);

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

    //
    // Metadata
    //

    /**
     * Set a metadata item.
     *
     * @param key
     * @param value
     */
    void setMetadata(const char *key, const char *value);

    /**
     * Set a metadata item.
     *
     * @param key
     * @param value
     */
    void getMetadata(const char *key, char **value);

    //
    // User
    //

    /**
     * Retrieve an user by it's ID.
     *
     * @param id
     * @param user
     */
    void getUser(const int id, User **user);

    /**
     * Retrieve an user by it's username.
     *
     * @param username
     * @param user
     */
    void getUser(const char *username, User **user);

    /**
     * Save a (new) user.
     *
     * @param key
     * @param value
     */
    void saveUser(User *user);

    /**
     * Delete an user.
     *
     * @param key
     * @param value
     */
    void deleteUser(const User *user);

    //
    // Keys
    //

    /**
     * Retrieve a key by it's ID.
     *
     * @param id
     * @param user
     */
    void getKey(const int id, Key **key);

    /**
     * Retrieve a key by it's inode and user ID.
     *
     * @param username
     * @param user
     */
    void getKey(const unsigned inode, const User *user, Key **key);

    /**
     * Save a (new) key.
     *
     * @param key
     * @param value
     */
    void saveKey(Key *key);

    /**
     * Delete a key.
     *
     * @param key
     * @param value
     */
    void deleteKey(const Key *key);

};


#endif //PARTITIONSAFE_KEYSTORE_H
