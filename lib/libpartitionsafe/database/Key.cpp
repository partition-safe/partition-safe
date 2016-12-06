//
// Created by Youri Lefers on 29-11-16.
//

#include <cstdlib>
#include "Key.h"
#include "../Common.h"
#include "../../libmbedtls/include/mbedtls/aes.h"
#include "../../libmbedtls/include/mbedtls/ctr_drbg.h"
#include "../../libmbedtls/include/mbedtls/entropy.h"

Key::Key(const unsigned id, const unsigned userId, const unsigned inode, const char *key):
    id(id), userId(userId), inode(inode), key(key) {}

Key *Key::create(const User *user, const char *password, const unsigned inode) {
    // Create the salted password
    char *saltedPassword;
    user->saltedPassword(password, user->salt, &saltedPassword);

    // The new encryption key
    unsigned char *encryptionKey;
    generateKey(&encryptionKey);

    // Encrypt the key
    char *encryptedKey;
    encrypt(saltedPassword, encryptionKey, &encryptedKey);

    // Encrypt the key
    char *decryptedKey;
    decrypt(saltedPassword, (unsigned char *)encryptedKey, &decryptedKey);

    // The new key
    return new Key(0, user->id, inode, encryptedKey);
}

void Key::generateKey(unsigned char **key) {
    Common::randomChars(ENCRYPTION_KEY_LENGTH, key);
}

void Key::encrypt(const char *saltedPassword, unsigned char *unEncryptedKey, char **encryptedKey) {

    //
    // Key generation
    //

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char key[32];

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) saltedPassword, strlen(saltedPassword)) != 0) throw "Huh";
    if(mbedtls_ctr_drbg_random(&ctr_drbg, key, 32 ) != 0) throw "Huh 2";

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16];
    unsigned char temp[strlen((const char*)unEncryptedKey) + 1];

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_enc(&aes, key, 256);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, strlen((const char*)unEncryptedKey) + 1, iv, unEncryptedKey, temp);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Copy to output
    *encryptedKey = (char *) malloc(sizeof(temp));
    strcpy(*encryptedKey, (char *) temp);
}

void Key::decrypt(const char *saltedPassword, unsigned char *encryptedKey, char **decryptedKey) {

    //
    // Key generation
    //

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char key[32];

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    if(mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *) saltedPassword, strlen(saltedPassword)) != 0) throw "Huh";
    if(mbedtls_ctr_drbg_random(&ctr_drbg, key, 32 ) != 0) throw "Huh 2";

    //
    // AES encryption
    //

    mbedtls_aes_context aes;
    unsigned char *iv = new unsigned char[16];
    unsigned char temp[strlen((const char*)encryptedKey) + 1];

    // Fill iv
    strncpy((char *)iv, saltedPassword, 16);

    // Initialize AES context
    mbedtls_aes_init(&aes);

    // Setup key
    mbedtls_aes_setkey_enc(&aes, key, 256);

    // Encrypt with CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, strlen((const char*)encryptedKey) + 1, iv, encryptedKey, temp);

    // Free up the AES context
    mbedtls_aes_free(&aes);

    // Copy to output
    *decryptedKey = (char *) malloc(sizeof(temp));
    strcpy(*decryptedKey, (char *) temp);
}
