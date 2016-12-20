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
                " `KEY` BLOB NOT NULL"
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

KeyStore::~KeyStore() {
    close();
}

void KeyStore::create(const char *path) {
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
    int res = sqlite3_close(sqliteHandle);
    if(res != SQLITE_OK) throw "Could not close SQLite handle";
}

sqlite3 *KeyStore::getSqliteHandle() {
    return sqliteHandle;
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

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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
    strcpy(*value, (char *) sqlite3_column_text(stmt, 0));

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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
    int _id = sqlite3_column_int(stmt, 0);

    // Read data
    std::string _username = std::string((char *)sqlite3_column_text(stmt, 1));
    std::string _salt = std::string((char *)sqlite3_column_text(stmt, 2));
    std::string _public = std::string((char *)sqlite3_column_text(stmt, 3));
    std::string _private = std::string((char *)sqlite3_column_text(stmt, 4));

    // Create the user
    *user = new User((const unsigned) _id, _username.c_str(), _salt.c_str(), _public.c_str(), _private.c_str());

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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

    // Read data
    std::string _username = std::string((char *)sqlite3_column_text(stmt, 1));
    std::string _salt = std::string((char *)sqlite3_column_text(stmt, 2));
    std::string _public = std::string((char *)sqlite3_column_text(stmt, 3));
    std::string _private = std::string((char *)sqlite3_column_text(stmt, 4));

    // Create the user
    *user = new User((const unsigned) _id, _username.c_str(), _salt.c_str(), _public.c_str(), _private.c_str());

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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
    std::string _key = std::string((char *)sqlite3_column_text(stmt, 3));

    // Create the user
    *key = new Key((const unsigned) _id, (const unsigned) _user, (const unsigned) _inode, (const unsigned char *) _key.c_str());

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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
    std::string _key = std::string((char *)sqlite3_column_text(stmt, 3));

    // Create the user
    *key = new Key((const unsigned) _id, (const unsigned) _user, (const unsigned) _inode, (const unsigned char *) _key.c_str());

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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
    if(sqlite3_bind_blob(stmt, index, (const void *)key->key, -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

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

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
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

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
}

//
// Notifications
//

int KeyStore::saveNotification(BaseNotification *notification) {
    sqlite3_stmt *stmt;
    int index;

    // Setup the query
    const char *query = "INSERT INTO NOTIFICATIONS (USER_FROM, USER_TO, TYPE, DETAILS) VALUES (:user_from, :user_to, :type, :details)";

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, query, -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":user_from")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, notification->user_from) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":user_to")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_int(stmt, index, notification->user_to) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":type")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_int(stmt, index, notification->type) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":details")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_text(stmt, index, notification->content.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";

    // Retrieve the last inserted row ID
    int id = static_cast<int>(sqlite3_last_insert_rowid(sqliteHandle));

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";

    // Return the ID
    return id;
}

void KeyStore::loadNotification(int id, BaseNotification **notification) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USER_FROM, USER_TO, TYPE, DETAILS FROM NOTIFICATIONS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const int _user_from = sqlite3_column_int(stmt, 1);
    const int _user_to = sqlite3_column_int(stmt, 2);
    const int _type = sqlite3_column_int(stmt, 3);
    std::string _details = std::string((char *)sqlite3_column_text(stmt, 4));

    // Create the user
    *notification = new BaseNotification(_id, _user_from, _user_to, _type, _details);

    // Finalize
    res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
}

void KeyStore::deleteNotification(BaseNotification *notification) {
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "DELETE FROM NOTIFICATIONS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, notification->id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";
}

std::vector<BaseNotification *> *KeyStore::loadNotificationsForUser(int user_id) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Prepare the statement
    if(sqlite3_prepare_v2(sqliteHandle, "SELECT ID, USER_FROM, USER_TO, TYPE, DETAILS FROM NOTIFICATIONS WHERE USER_TO = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, user_id) != SQLITE_OK) throw "Could not bind parameter";

    // Create vector
    std::vector<BaseNotification *> *vector = new std::vector<BaseNotification *>();

    // Iterate over all rows
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        // Get the result of the current row
        const int _id = sqlite3_column_int(stmt, 0);
        const int _user_from = sqlite3_column_int(stmt, 1);
        const int _user_to = sqlite3_column_int(stmt, 2);
        const int _type = sqlite3_column_int(stmt, 3);
        std::string _details = std::string((char *)sqlite3_column_text(stmt, 4));

        // Create the user
        BaseNotification *notification = new BaseNotification(_id, _user_from, _user_to, _type, _details);

        // Add notification to vector
        vector->push_back(notification);
    }

    // Finalize
    int res = sqlite3_finalize(stmt);
    if(res != SQLITE_OK) throw "Could not finalize statement";

    // Return the vector
    return vector;
}
