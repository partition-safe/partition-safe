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
    unsigned char *encryptedKey;
    encrypt(saltedPassword, encryptionKey, &encryptedKey);

    delete saltedPassword;
    delete encryptionKey;

    // The new key
    return new Key(0, user->id, inode, encryptedKey);
}

Key *Key::create(const User *user, const char *password, const unsigned char *encryptionKey, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // Encrypt the key
    unsigned char *encryptedKey;
    encrypt(saltedPassword, encryptionKey, &encryptedKey);

    // The new key
    return new Key(0, user->id, inode, encryptedKey);
}

void Key::generateKey(unsigned char **key) {
    Common::randomChars(ENCRYPTION_KEY_LENGTH, key);
}

void Key::encrypt(const char *saltedPassword, const unsigned char *unEncryptedKey, unsigned char **encryptedKey) {

    //
    // Key generation
    //

    unsigned char *key = (unsigned char *)calloc(ENCRYPTION_KEY_SIZE + 1, sizeof(unsigned char));
    if (key == NULL) throw "Could not allocate something";

    // Fill iv
    size_t saltedLenght = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLenght > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLenght);
    size_t lt = sizeof(key);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = (unsigned char *)calloc(16 + 1, sizeof(unsigned char));
    if (iv == NULL) throw "Could not allocate something";
    unsigned char *temp = (unsigned char *)calloc(strlen((const char*)encryptedKey) + 1, sizeof(unsigned char));
    if (temp == NULL) throw "Could not allocate something";

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
    *encryptedKey = (unsigned char *) calloc(ENCRYPTION_KEY_LENGTH + 1, sizeof(unsigned char));
    if (*encryptedKey == NULL) throw "Could not allocate something";
    strncpy((char *)*encryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH + 1);

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
}

void Key::decrypt(const char *saltedPassword, const unsigned char *encryptedKey, unsigned char **decryptedKey) {

    //
    // Key generation
    //

    unsigned char *key = (unsigned char *)calloc(ENCRYPTION_KEY_SIZE + 1, sizeof(unsigned char));
    if (key == NULL) throw "Could not allocate something";

    // Fill iv
    size_t saltedLenght = strlen(saltedPassword);
    strncpy((char *)key, saltedPassword,  saltedLenght > ENCRYPTION_KEY_SIZE ? ENCRYPTION_KEY_SIZE : saltedLenght);

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = (unsigned char *)calloc(16 + 1, sizeof(unsigned char));
    if (iv == NULL) throw "Could not allocate something";
    unsigned char *temp = (unsigned char *)calloc(strlen((const char*)encryptedKey) + 1, sizeof(unsigned char));
    if (temp == NULL) throw "Could not allocate something";

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
    *decryptedKey = (unsigned char *) calloc(ENCRYPTION_KEY_LENGTH + 1, sizeof(unsigned char));
    if (*decryptedKey == NULL) throw "Could not allocate something";
    strncpy((char *)*decryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH + 1);

    // Free memory
    delete[] key;
    delete[] iv;
    delete[] temp;
}
