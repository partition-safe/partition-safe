//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "User.h"
#include "../Common.h"
#include "Key.h"

User::User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey):
    id(id), username(new char[strlen(username) + 1]), salt(new char[strlen(salt) + 1]), publicKey(new char[strlen(publicKey) + 1]), privateKey(new char[strlen(privateKey) + 1]) {
    strncpy(this->username, username, strlen(username));
//    std::cout << "Salt: " << salt << " : " << strlen(salt) << std::endl;
    strncpy(this->salt, salt, ENCRYPTION_IV_LENGTH + 1);
    strncpy(this->publicKey, publicKey, 512 + 6 + 1);
    strncpy(this->privateKey, privateKey, 512 + 6 + 512 + (256 * 5) + 1);
}

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
