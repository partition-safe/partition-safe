//
// Created by Youri Lefers on 29-11-16.
//

#include "Key.h"
#include "../Common.h"

Key::Key(const unsigned id, const unsigned userId, const unsigned inode, const char *key):
    id(id), userId(userId), inode(inode), key(key) {}

Key *Key::create(const User *user, const char *password, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // The new encryption key
    char *encryptionKey;
    generateKey(&encryptionKey);

    // Encrypt the key
    char *encryptedKey;

    // The new key
    return new Key(0, user->id, inode, encryptionKey);
}

void Key::generateKey(char **key) {
    Common::randomChars(ENCRYPTION_KEY_LENGTH, key);
}
