//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include "User.h"
#include "../Common.h"

User::User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey):
    id(id), username(username), salt(salt), publicKey(publicKey), privateKey(privateKey) {}

User *User::create(const char *username, const char *password) {
    // Create a new salt
    char *salt;
    generateSalt(&salt);

    // Create salted password
    char* salted_password;
    saltedPassword(password, salt, &salted_password);

    // Create public and private keys
    char *pubKey = new char[1024];
    char *privKey = new char[3072];
    Common::createKeyPair(salted_password, &pubKey, &privKey);

    // Create the user
    User *user = new User(0, username, salt, pubKey, privKey);

    // Return the user
    return user;
}

void User::saltedPassword(const char *password, const char *salt, char **saltedPassword) {
    *saltedPassword = (char*)malloc(strlen(password) + strlen(salt) + 1);
    strcpy(*saltedPassword, password);
    strcat(*saltedPassword, salt);
}

void User::generateSalt(char **salt) {
    Common::randomChars(16, salt);
}
