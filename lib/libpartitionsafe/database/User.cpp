//
// Created by Youri Lefers on 29-11-16.
//

#include "User.h"

User::User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey):
    id(id), username(username), salt(salt), publicKey(publicKey), privateKey(privateKey) {}
