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
    id(id), userId(userId), inode(inode), key(new unsigned char[ENCRYPTION_KEY_LENGTH_BYTES + 1]) {
    std::cout << "KEY 1: " << key << " - " << strlen((const char *)key) << std::endl;

    // Convert key to std::string
    std::string t = std::string((const char *)key);
    std::cout << "KEY 2: " << t << " - " << std::to_string(t.length()) << std::endl;

    // Insert key into value
//    std::copy_n(t.begin(), ENCRYPTION_KEY_LENGTH_BYTES,  std::back_inserter(this->key));
    std::strncpy((char *)this->key, t.c_str(), ENCRYPTION_KEY_LENGTH_BYTES);
    std::cout << "KEY 2: " << this->key << " - " << strlen((const char *)this->key) << std::endl;



//    std::cout << "KEY 1: " << key << " - " << strlen((const char *)key) << std::endl;
//    std::copy(key, key + ENCRYPTION_KEY_LENGTH_BYTES, this->key);
//    std::cout << "KEY 2: " << this->key << " - " << strlen((const char *)this->key) << std::endl;
}

Key *Key::create(const User *user, const char *password, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // The new encryption key
    unsigned char *encryptionKey;
    generateKey(&encryptionKey);
    std::cout << "Encryption key: " << encryptionKey << std::endl;

    // Encrypt the key
    unsigned char *encrypted;
    encrypt(saltedPassword, encryptionKey, &encrypted);
    std::cout << "Encrypted key: " << encrypted << " : " << std::to_string(strlen((const char *)encrypted)) << std::endl;

    // Cleanup
#ifndef __WIN32
    delete[] saltedPassword;
    delete[] encryptionKey;
#endif

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
    Common::randomChars(ENCRYPTION_KEY_LENGTH_BYTES, key);
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
    *encryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*encryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH_BYTES);

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
    *decryptedKey = new unsigned char[ENCRYPTION_KEY_LENGTH + 1]();
    strncpy((char *)*decryptedKey, (char *)temp, ENCRYPTION_KEY_LENGTH_BYTES);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Free memory
    delete[] key;
    delete[] iv;
    delete[] temp;
}
