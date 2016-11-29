//
// Created by marc on 15-11-16.
//

#include "KeyStore.h"

KeyStore::KeyStore(const char *path, FILE *fh):
    path(path), fd(fh) {}

void KeyStore::create(const char *path) {
    // Open the file path
    FILE* fd = fopen(path, "w");

    // Check the file descriptor
    if(fd == nullptr) {
        throw "Could not open the file";
    }

    // Close the file descriptor
    fclose(fd);
}

KeyStore *KeyStore::init(const char *keyStorePath) {
    // Open the key store file
    FILE* keyStoreFileDescriptor = fopen(keyStorePath, "r+");

    // Check the descriptor
    if(keyStoreFileDescriptor == nullptr) {
        throw "Could not open key store file";
    }

    // Open the key store
    KeyStore* keyStore = new KeyStore(keyStorePath, keyStoreFileDescriptor);

    // Return the key store
    return keyStore;
}
