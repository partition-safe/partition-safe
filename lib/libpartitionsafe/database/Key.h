//
// Created by Youri Lefers on 29-11-16.
//

#ifndef PARTITIONSAFE_KEY_H
#define PARTITIONSAFE_KEY_H

#define ENCRYPTION_KEY_LENGTH 256

#include "User.h"

class Key {
public:
    /**
     * The ID of the user.
     */
    const unsigned id;

    /**
     * The username of the user.
     */
    const unsigned userId;

    /**
     * The salt of the user's password.
     */
    const unsigned inode;

    /**
     * The public key of the user.
     */
    const char *key;

    /**
     * User constructor.
     *
     * @param id
     * @param userId
     * @param inode
     * @param key
     */
    Key(const unsigned id, const unsigned userId, const unsigned inode, const char *key);

    /**
     * Create a new key for an user.
     *
     * @param user
     * @param password
     * @param inode
     * @return
     */
    static Key *create(const User *user, const char *password, const unsigned inode = 0);

    /**
     * Generate a new encryption key.
     *
     * @param key The generated key
     */
    static void generateKey(char **key);

};


#endif //PARTITIONSAFE_USER_H
