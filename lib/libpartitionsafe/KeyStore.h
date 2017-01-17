//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>
#include <vector>
#include "../libsqlite/sqlite3.h"
#include "database/User.h"
#include "database/Key.h"
#include "notification/BaseNotification.h"

class KeyStore {
    /**
     * File path to this partition
     */
    const char *path;

    /**
     * The SQLite handle.
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
     * Execute a query with error handling.
     *
     * @param db The database handle
     * @param query The query to execute
     */
    static void execute(sqlite3 *db, const char *query);

public:
    /**
     * Constructor of a key store.
     *
     * @param path The path to the key store
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
     */
    static void create(const char *path);

    /**
     * Open a key store instance.
     *
     * @param keyStorePath The path to the key store file
     *
     * @return The opened vault instance
     */
    static KeyStore *init(const char *keyStorePath);

    /**
     * Close the current key store instance.
     */
    void close();

    /**
     * Get the SQLite handle.
     *
     * @return The SQLite handle
     */
    sqlite3 *getSqliteHandle();

    //
    // Metadata
    //

    /**
     * Set a metadata item.
     *
     * @param key The key of the metadata item
     * @param value The value to store
     */
    void setMetadata(const char *key, const char *value);

    /**
     * Get a metadata item.
     *
     * @param key The key of the metadata item
     * @param value OUTPUT The value of the metadata item
     */
    void getMetadata(const char *key, char **value);

    //
    // User
    //

    /**
     * Retrieve all existing users.
     *
     * @param users OUTPUT The list of users
     */
    void getUsers(std::vector<User *> **users);

    /**
     * Retrieve an user by it's ID.
     *
     * @param id The ID of the user
     * @param user OUTPUT The retrieved user object
     */
    void getUser(const int id, User **user);

    /**
     * Retrieve an user by it's username.
     *
     * @param username The username of the user
     * @param user OUTPUT The retrieved user object
     */
    void getUser(const char *username, User **user);

    /**
     * Save a (new) user.
     *
     * @param user The user to save
     */
    void saveUser(User *user);

    /**
     * Delete an user.
     *
     * @param user The user to delete
     */
    void deleteUser(const User *user);

    //
    // Keys
    //

    /**
     * Retrieve a key by it's ID.
     *
     * @param id The ID of the key
     * @param key OUTPUT The retrieved key object
     */
    void getKey(const int id, Key **key);

    /**
     * Retrieve a key by it's path and user ID.
     *
     * @param path The path of this key
     * @param user The user of this key
     * @param key OUTPUT The retrieved key object
     */
    void getKey(const char *path, const User *user, Key **key);

    /**
     * Save a (new) key.
     *
     * @param key The key to save
     */
    void saveKey(Key *key);

    /**
     * Delete a key.
     *
     * @param key The key to delete
     */
    void deleteKey(const Key *key);

    //
    // Notifications
    //

    /**
     * Save a new notification.
     *
     * @param notification The notification to save
     *
     * @return The notification ID
     */
    int saveNotification(BaseNotification *notification);

    /**
     * Load a simple notification.
     *
     * @param id The ID of the notification
     * @param notification OUTPUT The retrieved notification object
     */
    void loadNotification(int id, BaseNotification **notification);

    /**
     * Delete a notification.
     *
     * @param notification The notification to delete
     */
    void deleteNotification(BaseNotification *notification);

    /**
     * Load all notifications for a specific user.
     *
     * @param user_id The user ID of the user to retrieve notification for
     *
     * @return The vector of found notifications
     */
    std::vector<BaseNotification *> *loadNotificationsForUser(int user_id);

};


#endif //PARTITIONSAFE_KEYSTORE_H
