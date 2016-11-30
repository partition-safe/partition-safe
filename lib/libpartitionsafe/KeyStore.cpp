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
    char *zErrMsg = 0;

    // Open the SQlite 3 database path
    int rc = sqlite3_open(path, &db);
    if(rc) throw "Could not open the database for writing";

    // Keep track of thrown errors
    bool errorThrown = false;

    try {
        // Create metadata table
        rc = sqlite3_exec(db, STMT_CREATE_TABLE_METADATA, nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) throw zErrMsg;

        // Create metadata table
        rc = sqlite3_exec(db, STMT_CREATE_TABLE_USERS, nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) throw zErrMsg;

        // Create metadata table
        rc = sqlite3_exec(db, STMT_CREATE_TABLE_KEYS, nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) throw zErrMsg;

        // Create metadata table
        rc = sqlite3_exec(db, STMT_CREATE_TABLE_NOTIFICATIONS, nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) throw zErrMsg;
    } catch(...) {
        // Free up the error message
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);

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

User *KeyStore::createUser(const char *username) {
    // Create all fields

    // Create key pairs
    unsigned char *pubKey;
    unsigned char *privKey;
    createKeyPair(&pubKey, &privKey);

    // Create the new user object
    User *user = new User(0, username, "", (const char *)pubKey, (const char *)privKey);

    // Return the user
    return user;
}

mbedtls_rsa_context KeyStore::createKeyPair(unsigned char* *pubKey, unsigned char* *privKey) {
    int ret;
    mbedtls_rsa_context rsa;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "rsa_genkey";

    // Initialize
    mbedtls_ctr_drbg_init( &ctr_drbg );

    // Setup entropy (feed random generator)
    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                                       (const unsigned char *) pers,
                                       strlen( pers ) ) ) != 0 )
    {
        throw "Could not create seed: " + ret;
    }

    // Create RSA key
    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
    if( ( ret = mbedtls_rsa_gen_key( &rsa, mbedtls_ctr_drbg_random, &ctr_drbg, KEY_SIZE, EXPONENT ) ) != 0 )
    {
        throw "Could not create key: " + ret;
    }

    // Freeup some space
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    // Return rsa
    return rsa;
}
