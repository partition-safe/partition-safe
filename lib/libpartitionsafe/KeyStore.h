//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_KEYSTORE_H
#define PARTITIONSAFE_KEYSTORE_H


#include <cstdio>

class KeyStore {
    /**
     * File path to this partition
     */
    const char* path;

    /**
     * The file descriptor of this partition.
     *
     * This descriptor contains the real partition file.
     */
    FILE * fd;

public:
    /**
     * Constructor of a key store.
     *
     * @param path
     * @param fh
     * @return
     */
    KeyStore(const char* path, FILE* fh = nullptr);

    /**
     * Create a new key store.
     *
     * @param path The path to place the new key store
     *
     * @return
     */
    static void create(const char* path);

    /**
     * Open a key store instance.
     *
     * @param keyStorePath The path to the key store file
     *
     * @return The opened vault instance
     */
    static KeyStore* init(const char* keyStorePath);

};


#endif //PARTITIONSAFE_KEYSTORE_H
