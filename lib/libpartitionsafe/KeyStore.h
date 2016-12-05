//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>
#include "../libsqlite/sqlite3.h"
#include "database/User.h"

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

    //
    // Database handling
    //

    /**
     * Execute an query with error handling.
     *
     * @param db The sqlite instance
     * @param query The query to run
     */
    static void execute(sqlite3 *db, const char *query);

    /**
     * Prepare a SQL statement.
     *
     * @param db
     * @param stmt
     * @param sql
     */
    static void prepare(sqlite3 *db, sqlite3_stmt *stmt, const char *sql);

    /**
     * Bind a param for a prepared SQL statement.
     *
     * @param stmt
     * @param key
     * @param value
     */
    static void bindParam(sqlite3_stmt *stmt, const char *key, const char *value);

    /**
     * Bind a param for a prepared SQL statement.
     *
     * @param stmt
     * @param key
     * @param value
     */
    static void bindParam(sqlite3_stmt *stmt, const char *key, const int value);

    /**
     * Execute a statement with error handling.
     *
     * @param stmt The statement
     */
    static void execute(sqlite3_stmt *stmt);

    /**
     * Finalize the statement.
     *
     * @param stmt
     */
    static void close(sqlite3_stmt *stmt);

};


#endif //PARTITIONSAFE_KEYSTORE_H
