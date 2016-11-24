//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITION_H
#define PARTITIONSAFE_PARTITION_H

#include <iostream>
#include <vector>
#include "../libfatfs/src/ff.h"
#include "Entry.h"

class Partition {
    /**
     * The save version for compatibility checks.
     */
    static const TCHAR* LETTER;

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

    /**
     * The FatFS instance.
     */
    FATFS fs;

public:
    /**
     * The identifier of the save.
     *
     * Used for testing decryption.
     */
    static const char* IDENTIFIER;

    /**
     * The save version for compatibility checks.
     */
    static const unsigned int VERSION;

    /**
     * Constructor of a partition.
     *
     * @param path
     * @param fh
     * @return
     */
    Partition(const char* path, FILE* fh = nullptr);

    /**
     * Open the partition.
     *
     * @param path The path to the partition file
     *
     * @return
     */
    Partition* open();

    /**
     * Create a new partition.
     *
     * @return
     */
    Partition* create();

    /**
     * Write to a file on the partition.
     *
     * @param fileName
     * @param buff
     * @param size
     *
     * @return
     */
    Partition* writeFile(const TCHAR* fileName, const void* buff, const UINT size);

    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    Partition* fileInfo(const TCHAR *fileName, FILINFO *fileInfo);

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName
     * @param buff
     *
     * @return
     */
    Partition* readFile(const TCHAR *fileName, void *buff, const UINT size);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return
     */
    Partition* createDirectory(const TCHAR *directoryName);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return
     */
    Partition* createDirectory(const std::string directoryName);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return
     */
    Partition* createDirectory(const char *directoryName);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const TCHAR *directoryName);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const std::string directoryName);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const char *directoryName);

};

#endif //PARTITIONSAFE_PARTITION_H
