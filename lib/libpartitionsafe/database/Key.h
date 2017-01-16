//
// Created by Youri Lefers on 29-11-16.
//

#ifndef PARTITIONSAFE_KEY_H
#define PARTITIONSAFE_KEY_H

#define ENCRYPTION_KEY_LENGTH 256
#define ENCRYPTION_KEY_LENGTH_BYTES ENCRYPTION_KEY_LENGTH / 8
#define ENCRYPTION_KEY_SIZE 64
#define ENCRYPTION_KEY_BITS 256
#define ENCRYPTION_IV_LENGTH 16

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
     * The path of the key.
     */
    char *path;

    /**
     * The public key of the user.
     */
    unsigned char *key;

    /**
     * User constructor.
     *
     * @param id
     * @param userId
     * @param path
     * @param key
     */
    Key(const unsigned id, const unsigned userId, const char *path, const unsigned char *key);

    /**
     * Create a new key for an user.
     *
     * @param user
     * @param password
     * @param path
     * @return
     */
    static Key *create(const User *user, const char *password, const char *path = "/");

    /**
     * Create a new key for an user based on an existing key.
     *
     * @param user
     * @param password
     * @param path
     * @return
     */
    static Key *create(const User *user, const char *password, const unsigned char *encryptionKey, const char *path = "/");

    /**
     * Generate a new encryption key.
     *
     * @param key The generated key
     */
    static void generateKey(unsigned char **key);

    /**
     * Encrypt the key.
     *
     * @param saltedPassword The salted password of the user
     * @param unEncryptedKey The unencrypted key of the user to encrypt
     * @param encryptedKey The encrypted key as output
     */
    static void encrypt(const char *saltedPassword, const unsigned char unEncryptedKey[ENCRYPTION_KEY_LENGTH], unsigned char **encryptedKey);

    /**
     * Decrypt the key.
     *
     * @param saltedPassword The salted password of the user
     * @param encryptedKey The encrypted key to decrypt
     * @param decryptedKey The decrypted key as output
     */
    static void decrypt(const User *user, const char *password, const unsigned char encryptedKey[ENCRYPTION_KEY_LENGTH], unsigned char **decryptedKey);

    /**
     * Decrypt the key.
     *
     * @param saltedPassword The salted password of the user
     * @param encryptedKey The encrypted key to decrypt
     * @param decryptedKey The decrypted key as output
     */
    static void decrypt(const char *saltedPassword, const unsigned char encryptedKey[ENCRYPTION_KEY_LENGTH], unsigned char **decryptedKey);

};


#endif //PARTITIONSAFE_USER_H
