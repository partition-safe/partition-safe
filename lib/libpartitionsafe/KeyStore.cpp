//
// Created by marc on 15-11-16.
//

#include <cstring>
#include "KeyStore.h"

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
        sqlite3_stmt *stmt;

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
    // Run the query
    int rc = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (rc != SQLITE_OK) throw "SQL error thrown, check debug output";
}

//
// Metadata
//

void KeyStore::setMetadata(const char *key, const char *value) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "INSERT OR REPLACE INTO METADATA (KEY, VALUE) VALUES (:key, :value)", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":key")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, key, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":value")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, value, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";
}

void KeyStore::getMetadata(const char *key, char **value) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT VALUE FROM METADATA WHERE KEY = :key", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":key")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, key, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_OK && res != SQLITE_DONE && res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const char *tempVal = (const char *) sqlite3_column_text(stmt, 0);

    // Write the result
    strncpy(*value, tempVal, sizeof(tempVal));
}

//
// User
//

void KeyStore::saveUser(User *user) {
    sqlite3_stmt *stmt;
    const char *query;
    int index;

    // Setup the query
    if(user->id == 0) {
        query = "INSERT INTO USERS (USERNAME, SALT, PUBLIC_KEY, PRIVATE_KEY) VALUES (:username, :salt, :public, :private)";
    } else {
        query = "UPDATE USERS SET USERNAME = :username WHERE ID = :id";
    }

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, query, -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":username")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, user->username, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // New user?
    if(user->id == 0) {
        if ((index = sqlite3_bind_parameter_index(stmt, ":salt")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_text(stmt, index, user->salt, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";
        if ((index = sqlite3_bind_parameter_index(stmt, ":public")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_text(stmt, index, user->publicKey, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";
        if ((index = sqlite3_bind_parameter_index(stmt, ":private")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_text(stmt, index, user->privateKey, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";
    } else {
        if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_int(stmt, index, user->id) != SQLITE_OK) throw "Could not bind parameter";
    }

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";
}

void KeyStore::deleteUser(const User *user) {
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "DELETE FROM USERS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, user->id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";
}

void KeyStore::getUser(const int id, User **user) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USERNAME, SALT, PUBLIC_KEY, PRIVATE_KEY FROM USERS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const char *_username = (const char *) sqlite3_column_text(stmt, 1);
    const char *_salt = (const char *) sqlite3_column_text(stmt, 2);
    const char *_public = (const char *) sqlite3_column_text(stmt, 3);
    const char *_private = (const char *) sqlite3_column_text(stmt, 4);

    // Create the user
    *user = new User((const unsigned) _id, _username, _salt, _public, _private);
}

void KeyStore::getUser(const char *username, User **user) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USERNAME, SALT, PUBLIC_KEY, PRIVATE_KEY FROM USERS WHERE USERNAME = :username", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":username")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, username, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const char *_username = (const char *) sqlite3_column_text(stmt, 1);
    const char *_salt = (const char *) sqlite3_column_text(stmt, 2);
    const char *_public = (const char *) sqlite3_column_text(stmt, 3);
    const char *_private = (const char *) sqlite3_column_text(stmt, 4);

    // Create the user
    *user = new User((const unsigned) _id, _username, _salt, _public, _private);
}

//
// Keys
//

void KeyStore::getKey(const int id, Key **key) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USER, INODE, KEY FROM KEYS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const int _user = sqlite3_column_int(stmt, 1);
    const int _inode = sqlite3_column_int(stmt, 2);
    const char *_key = (const char *) sqlite3_column_text(stmt, 3);

    // Create the user
    *key = new Key((const unsigned) _id, (const unsigned) _user, (const unsigned) _inode, _key);
}

void KeyStore::getKey(const unsigned inode, const User *user, Key **key) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USER, INODE, KEY FROM KEYS WHERE INODE = :inode AND USER = :user", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":inode")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, inode) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":user")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, user->id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const int _user = sqlite3_column_int(stmt, 1);
    const int _inode = sqlite3_column_int(stmt, 2);
    const char *_key = (const char *) sqlite3_column_text(stmt, 3);

    // Create the user
    *key = new Key((const unsigned) _id, (const unsigned) _user, (const unsigned) _inode, _key);
}

void KeyStore::saveKey(Key *key) {
    sqlite3_stmt *stmt;
    const char *query;
    int index;

    // Setup the query
    if(key->id == 0) {
        query = "INSERT INTO KEYS (USER, INODE, KEY) VALUES (:user, :inode, :key)";
    } else {
        query = "UPDATE KEYS SET KEY = :key WHERE ID = :id";
    }

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, query, -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":key")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_text(stmt, index, key->key, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // New user?
    if(key->id == 0) {
        if ((index = sqlite3_bind_parameter_index(stmt, ":user")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_int(stmt, index, key->userId) != SQLITE_OK) throw "Could not bind parameter";
        if ((index = sqlite3_bind_parameter_index(stmt, ":inode")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_int(stmt, index, key->inode) != SQLITE_OK) throw "Could not bind parameter";
    } else {
        if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
        if (sqlite3_bind_int(stmt, index, key->id) != SQLITE_OK) throw "Could not bind parameter";
    }

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";
}

void KeyStore::deleteKey(const Key *key) {
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "DELETE FROM KEYS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, key->id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";
}
