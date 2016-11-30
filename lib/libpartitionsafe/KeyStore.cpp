//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "KeyStore.h"
#include "../libmbedtls/include/mbedtls/entropy.h"
#include "../libmbedtls/include/mbedtls/ctr_drbg.h"

const char *KeyStore::STMT_CREATE_TABLE_METADATA =
        "CREATE TABLE `METADATA` ("
                " `KEY` TEXT NOT NULL UNIQUE,"
                " `VALUE` TEXT NOT NULL,"
                " PRIMARY KEY(`KEY`)"
                ");";

const char *KeyStore::STMT_CREATE_TABLE_USERS =
        "CREATE TABLE `USERS` ("
                " `ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
                " `USERNAME` TEXT NOT NULL UNIQUE,"
                " `SALT` TEXT NOT NULL UNIQUE,"
                " `PUBLIC_KEY` TEXT NOT NULL UNIQUE,"
                " `PRIVATE_KEY` TEXT NOT NULL UNIQUE"
                ");";

const char *KeyStore::STMT_CREATE_TABLE_KEYS =
        "CREATE TABLE `KEYS` ("
                " `ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
                " `USER` INTEGER NOT NULL,"
                " `INODE` INTEGER NOT NULL,"
                " `KEY` INTEGER NOT NULL"
                ");";

const char *KeyStore::STMT_CREATE_TABLE_NOTIFICATIONS =
        "CREATE TABLE `NOTIFICATIONS` ("
                " `ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
                " `USER_FROM` INTEGER NOT NULL,"
                " `USER_TO` INTEGER NOT NULL,"
                " `TYPE` INTEGER NOT NULL,"
                " `DETAILS` TEXT"
                ");";

KeyStore::KeyStore(const char *path):
    path(path) {
    // Open the SQlite 3 database path
    int result = sqlite3_open(path, &sqliteHandle);
    if(result) throw "Could not open the database";
}

KeyStore *KeyStore::create(const char *path) {
    // Setup the handle for creation
    sqlite3 *db;

    // Open the SQlite 3 database path
    int rc = sqlite3_open(path, &db);
    if(rc) throw "Could not open the database for writing";

    // Keep track of thrown errors
    bool errorThrown = false;

    try {
        // Create tables
        execute(db, STMT_CREATE_TABLE_METADATA);
        execute(db, STMT_CREATE_TABLE_USERS);
        execute(db, STMT_CREATE_TABLE_KEYS);
        execute(db, STMT_CREATE_TABLE_NOTIFICATIONS);
    } catch(...) {
        // Set error thrown
        errorThrown = true;
    }

    // Close the connection
    sqlite3_close(db);

    // Error thrown?
    if(errorThrown) throw "SQL error thrown, check debug output";

    // Create and return the new instance
    return init(path);
}

KeyStore *KeyStore::init(const char *keyStorePath) {
    // Open the key store
    KeyStore* keyStore = new KeyStore(keyStorePath);

    // Return the key store
    return keyStore;
}

void KeyStore::close() {
    // Close the connection
    sqlite3_close(sqliteHandle);
}

//
// Database handling
//

void KeyStore::execute(sqlite3 *db, const char *query) {
    // Prepare the query
    sqlite3_stmt *stmt;
    prepare(&db, &stmt, query);

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK) throw "SQL error thrown, check debug output";
}

void KeyStore::prepare(sqlite3 **db, sqlite3_stmt **stmt, const char *sql) {
    if (*stmt == NULL) {
        // Prepare the stement
        int res = sqlite3_prepare_v2(*db, sql, -1, stmt, NULL);
        if(res != SQLITE_OK) throw "Could not prepare a statement";
    }
}

void KeyStore::bindParam(sqlite3_stmt **stmt, const char *key, const char *value) {
    // Retrieve the index
    int index = sqlite3_bind_parameter_index(*stmt, key);
    if (index <= 0) throw "Could not retrieve parameter index in the statement";

    // Bind param
    int res = sqlite3_bind_text(*stmt, index, value, -1, SQLITE_TRANSIENT);
    if(res != SQLITE_OK) throw "Could not bind parameter";
}

void KeyStore::bindParam(sqlite3_stmt **stmt, const char *key, const int value) {
    // Retrieve the index
    int index = sqlite3_bind_parameter_index(*stmt, key);
    if (index <= 0) throw "Could not retrieve parameter index in the statement";

    // Bind param
    int res = sqlite3_bind_int(*stmt, index, value);
    if(res != SQLITE_OK) throw "Could not bind parameter";
}

void KeyStore::execute(sqlite3_stmt **stmt) {
    // Execute query
    int rc = sqlite3_step(*stmt);
    if (rc != SQLITE_OK) throw "Could not execute query";
}

//
// Metadata
//

void KeyStore::setMetadata(const char *key, const char *value) {
    // Prepare the query
    sqlite3_stmt *stmt;
    prepare(&sqliteHandle, &stmt, "INSERT OR REPLACE INTO METADATA (KEY, VALUE) VALUES (:key, :value);");

    // Bind parameters
    bindParam(&stmt, "key", key);
    bindParam(&stmt, "key", value);

    // Execute query and retrieve result
    execute(&stmt);
}

void KeyStore::getMetadata(const char *key, char **value) {
    // Prepare the query
    sqlite3_stmt *stmt;
    prepare(&sqliteHandle, &stmt, "SELECT VALUE FROM METADATA WHERE KEY = :key;");

    // Bind parameters
    bindParam(&stmt, "key", key);

    // Execute query and retrieve result
    execute(&stmt);

    // Get the result of the current row
    const char *tempVal = (const char *) sqlite3_column_text(stmt, 0);

    // Write the result
    strncpy(*value, tempVal, sizeof(tempVal));

    // Delete temp val
    delete tempVal;
}
