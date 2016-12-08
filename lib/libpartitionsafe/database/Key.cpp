//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include <iostream>
#include "Key.h"
#include "../Common.h"
#include "../../libmbedtls/include/mbedtls/aes.h"

Key::Key(const unsigned id, const unsigned userId, const unsigned inode, const unsigned char *key):
    id(id), userId(userId), inode(inode), key(key) {}

Key *Key::create(const User *user, const char *password, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // The new encryption key
    unsigned char *encryptionKey;
    generateKey(&encryptionKey);

    // Encrypt the key
    unsigned char *encrypted;
    encrypt(saltedPassword, encryptionKey, &encrypted);

//    delete[] saltedPassword;
//    delete[] encryptionKey;

    // The new key
    return new Key(0, user->id, inode, encrypted);
}

Key *Key::create(const User *user, const char *password, const unsigned char *encryptionKey, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Encrypt the key
    unsigned char *encrypted = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    encrypt(saltedPassword, encryptionKey, &encrypted);

    // The new key
    return new Key(0, user->id, inode, encrypted);
}

void Key::generateKey(unsigned char **key) {
    Common::randomChars(ENCRYPTION_KEY_LENGTH, key);
}

void Key::encrypt(const char *saltedPassword, const unsigned char *unEncryptedKey, unsigned char **encryptedKey) {

    //
    // Key generation
    //

    unsigned char *key = new unsigned char[ENCRYPTION_KEY_SIZE + 1]();

    // Fill iv
    size_t saltedLenght = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLenght > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLenght);
    size_t lt = sizeof(key);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16 + 1]();
    unsigned char *temp = new unsigned char[strlen((const char *) encryptedKey) + 1]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_enc(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, ENCRYPTION_KEY_LENGTH, iv, unEncryptedKey, temp);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Copy to output
    *encryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*encryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH);

    // Free memory
//    delete[] key;
//    delete[] iv;
//    delete[] temp;
}

void Key::decrypt(const User *user, const char *password, const unsigned char *encryptedKey, unsigned char **decryptedKey) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Get the decrypted key
    decrypt(saltedPassword, encryptedKey, decryptedKey);
}

void Key::decrypt(const char *saltedPassword, const unsigned char *encryptedKey, unsigned char **decryptedKey) {

    //
    // Key generation
    //

    unsigned char *key = new unsigned char[ENCRYPTION_KEY_SIZE + 1]();

    // Fill iv
    size_t saltedLenght = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLenght > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLenght);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16 + 1]();
    unsigned char *temp = new unsigned char[strlen((const char *) encryptedKey) + 1]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_dec(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, ENCRYPTION_KEY_LENGTH, iv, encryptedKey, temp);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Copy to output
    *decryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*decryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH);

    // Free memory
//    delete[] key;
//    delete[] iv;
//    delete[] temp;
}
