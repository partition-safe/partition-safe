//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "Key.h"
#include "../Common.h"
#include "../../libmbedtls/include/mbedtls/aes.h"

Key::Key(const unsigned id, const unsigned userId, const char *path, const unsigned char *key):
    id(id), userId(userId), path(new char[strlen(path) + 1]), key(new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES + 1]) {
    strncpy(this->path, path, strlen(path));
    strncpy((char *)this->key, (char *)key, ENCRYPTION_KEY_LENGTH_BYTES);
}

Key *Key::create(const User *user, const char *password, const char *path) {
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
#ifndef __WIN32
    delete[] saltedPassword;
    delete[] encryptionKey;
#endif

    // The new key
    Key *key = new Key(0, user->id, path, encrypted);
    return key;
}

Key *Key::create(const User *user, const char *password, const unsigned char *encryptionKey, const char *path) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Encrypt the key
    unsigned char *encrypted;
    encrypt(saltedPassword, encryptionKey, &encrypted);

    // Remove salted password
    delete saltedPassword;

    // The new key
    return new Key(0, user->id, path, encrypted);
}

void Key::generateKey(unsigned char **key) {
    Common::randomChars(ENCRYPTION_KEY_LENGTH_BYTES + 1, key);
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
    unsigned char *iv = new unsigned char[ENCRYPTION_IV_LENGTH]();
    unsigned char *temp = new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, ENCRYPTION_IV_LENGTH);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_enc(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, ENCRYPTION_KEY_LENGTH_BYTES, iv, unEncryptedKey, temp);

    // Copy to output
    *encryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES + 1]();
    strncpy((char *)*encryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH_BYTES);
    (*encryptedKey)[ENCRYPTION_KEY_LENGTH_BYTES] = 0x00;

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

#ifndef __WIN32
    // Remove salted password
    delete[] saltedPassword;
#endif
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
    unsigned char *iv = new unsigned char[ENCRYPTION_IV_LENGTH]();
    unsigned char *temp = new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES]();

    // Fill iv
    strncpy((char *)iv, saltedPassword, ENCRYPTION_IV_LENGTH);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_dec(&aes, key, ENCRYPTION_KEY_BITS);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, ENCRYPTION_KEY_LENGTH_BYTES, iv, encryptedKey, temp);

    // Copy to output
    *decryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES + 1]();
    strncpy((char *)*decryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH_BYTES);
    (*decryptedKey)[ENCRYPTION_KEY_LENGTH_BYTES] = 0x00;

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Free memory
    delete[] key;
    delete[] iv;
    delete[] temp;
}
