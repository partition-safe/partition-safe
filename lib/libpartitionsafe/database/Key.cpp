//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
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

    // Cleanup
    delete[] saltedPassword;
    delete[] encryptionKey;

    // The new key
    Key *key = new Key(0, user->id, inode, encrypted);
    return key;
}

Key *Key::create(const User *user, const char *password, const unsigned char *encryptionKey, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Encrypt the key
    unsigned char *encrypted;
    encrypt(saltedPassword, encryptionKey, &encrypted);

    // Remove salted password
    delete saltedPassword;

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

    unsigned char *key = new unsigned char[ENCRYPTION_KEY_SIZE]();

    // Fill iv
    size_t saltedLength = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLength > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLength);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16]();
    unsigned char *temp = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_enc(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, ENCRYPTION_KEY_LENGTH, iv, unEncryptedKey, temp);

    // Copy to output
    *encryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*encryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Free memory
    delete[] key;
    delete[] iv;
    delete[] temp;
}

void Key::decrypt(const User *user, const char *password, const unsigned char *encryptedKey, unsigned char **decryptedKey) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Get the decrypted key
    decrypt(saltedPassword, encryptedKey, decryptedKey);

    // Remove salted password
    delete saltedPassword;
}

void Key::decrypt(const char *saltedPassword, const unsigned char *encryptedKey, unsigned char **decryptedKey) {

    //
    // Key generation
    //

    unsigned char *key = new unsigned char[ENCRYPTION_KEY_SIZE]();

    // Fill iv
    size_t saltedLength = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLength > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLength);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16]();
    unsigned char *temp = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_dec(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, ENCRYPTION_KEY_LENGTH, iv, encryptedKey, temp);

    // Copy to output
    *decryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*decryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Free memory
    delete[] key;
    delete[] iv;
    delete[] temp;
}
