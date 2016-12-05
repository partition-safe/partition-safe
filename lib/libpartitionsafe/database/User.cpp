//
// Created by Youri Lefers on 29-11-16.
//

#include "User.h"
#include "../Common.h"

User::User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey):
    id(id), username(username), salt(salt), publicKey(publicKey), privateKey(privateKey) {}

User *User::create(const char *username, const char *password) {
    // Create a new salt
    const char *salt = "";

    // Create public and private keys
    char *pubKey = new char[1024];
    char *privKey = new char[3072];
//    Common::createKeyPair(pubKey, privKey, password);

    // Create the user
    User *user = new User(0, username, salt, pubKey, privKey);

    // Return the user
    return user;
}
