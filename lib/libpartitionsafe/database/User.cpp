//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include <cstring>
#include "User.h"
#include "../Common.h"
#include "Key.h"

User::User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey):
    id(id), username(username), salt(salt), publicKey(publicKey), privateKey(privateKey) {}

User *User::create(const char *username, const char *password) {
    // Create a new salt
    char *salt;
    generateSalt(&salt);

    // Create salted password
    char *salted_password;
    saltedPassword(password, salt, &salted_password);

    // Create public and private keys
    char *pubKey;
    char *privKey;
    Common::createKeyPair(salted_password, &pubKey, &privKey);

    // Create the user
    User *user = new User(0, username, salt, pubKey, privKey);

#ifndef __WIN32
    delete[] salted_password;
#endif

    // Return the user
    return user;
}

void User::saltedPassword(const char *password, const char *salt, char **saltedPassword) {
    *saltedPassword = new char[strlen(password) + strlen(salt)]();
    strcpy(*saltedPassword, password);
    strcat(*saltedPassword, salt);
}

void User::generateSalt(char **salt) {
    Common::randomChars(ENCRYPTION_IV_LENGTH, salt);
}
