//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_PARTITION_H
#define PARTITIONSAFE_PARTITION_H

#include <iostream>
#include <vector>
#include "../libfatfs/src/ff.h"
#include "filesystem/TreeEntry.h"

class Partition {
    /**
     * The save version for compatibility checks.
     */
    static const TCHAR *LETTER;

    /**
     * File path to this partition
     */
    const char *path;

    /**
     * The file descriptor of this partition.
     *
     * This descriptor contains the real partition file.
     */
    FILE *fd;

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
    static const unsigned char *IDENTIFIER;

    /**
     * The save version for compatibility checks.
     */
    static const unsigned int VERSION;

    /**
     * Constructor of a partition.
     *
     * @param path The path of the partition
     * @param fh The file handler
     */
    Partition(const char *path, FILE *fh = nullptr);

    /**
     * Partition destructor.
     */
    ~Partition();

    /**
     * Open the partition.
     *
     * @return The partition instance
     */
    Partition *open();

    /**
     * Create a new partition.
     *
     * @return The partition instance
     */
    Partition *create();

    /**
     * Write to a file on the partition.
     *
     * @param fileName The filename of the file
     * @param buff The buffer to write
     * @param size The size to write
     *
     * @return The partition instance
     */
    Partition *writeFile(const TCHAR *fileName, const void *buff, const UINT size);

    /**
     * Write to a file on the partition.
     *
     * @param fileName The filename of the file
     * @param buff The buffer to write
     * @param size The size to write
     *
     * @return The partition instance
     */
    Partition *writeFile(const std::string fileName, const void *buff, const UINT size);

#ifndef __WIN32

    /**
     * Write to a file on the partition.
     *
     * @param fileName The filename of the file
     * @param buff The buffer to write
     * @param size The size to write
     *
     * @return The partition instance
     */
    Partition *writeFile(const char *fileName, const void *buff, const UINT size);

#endif

    /**
     * Rename a file or folder.
     *
     * @param oldPath Path to the file or folder to be renamed
     * @param newPath Path to the file or folder with changed name
     *
     * @return The partition instance
     */
    Partition *renameFileFolder(const char *oldPath, const char *newPath);

    /**
     * Get information about a file.
     *
     * @param fileName The file to get information about
     * @param fileInfo OUTPUT The file info object to populate
     *
     * @return The partition instance
     */
    Partition *fileInfo(const TCHAR *fileName, FILINFO *fileInfo);

    /**
     * Get information about a file.
     *
     * @param fileName The file to get information about
     * @param fileInfo OUTPUT The file info object to populate
     *
     * @return The partition instance
     */
    Partition *fileInfo(const std::string fileName, FILINFO *fileInfo);

#ifndef __WIN32

    /**
     * Get information about a file.
     *
     * @param fileName The file to get information about
     * @param fileInfo OUTPUT The file info object to populate
     *
     * @return The partition instance
     */
    Partition *fileInfo(const char *fileName, FILINFO *fileInfo);

#endif

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName The file to read
     * @param buff OUTPUT The buffer to read into
     * @param size The size of the buffer
     *
     * @return The partition instance
     */
    Partition *readFile(const TCHAR *fileName, void *buff, const UINT size);

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName The file to read
     * @param buff OUTPUT The buffer to read into
     * @param size The size of the buffer
     *
     * @return The partition instance
     */
    Partition *readFile(const std::string fileName, void *buff, const UINT size);

#ifndef __WIN32

    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName The file to read
     * @param buff OUTPUT The buffer to read into
     * @param size The size of the buffer
     *
     * @return The partition instance
     */
    Partition *readFile(const char *fileName, void *buff, const UINT size);

#endif

    /**
     * Create a new directory.
     *
     * @param directoryName The directory to create
     *
     * @return The partition instance
     */
    Partition *createDirectory(const TCHAR *directoryName);

    /**
     * Create a new directory.
     *
     * @param directoryName The directory to create
     *
     * @return The partition instance
     */
    Partition *createDirectory(const std::string directoryName);

#ifndef __WIN32

    /**
     * Create a new directory.
     *
     * @param directoryName The directory to create
     *
     * @return The partition instance
     */
    Partition *createDirectory(const char *directoryName);

#endif

    /**
     * Get the content of a directory.
     *
     * @param directoryName The directory name to list
     * @param parentEntry The parent entry of the directory
     * @param directoriesOnly true to list only directories, defauls to false
     *
     * @return The list of found entries
     */
    std::vector<Entry *> *listDirectory(const TCHAR *directoryName, TreeEntry **parentEntry = nullptr,
                                        const bool directoriesOnly = false);

    /**
     * Get the content of a directory.
     *
     * @param directoryName The directory name to list
     * @param parentEntry The parent entry of the directory
     * @param directoriesOnly true to list only directories, defauls to false
     *
     * @return The list of found entries
     */
    std::vector<Entry *> *listDirectory(const std::string directoryName, TreeEntry **parentEntry = nullptr,
                                        const bool directoriesOnly = false);

#ifndef __WIN32

    /**
     * Get the content of a directory.
     *
     * @param directoryName The directory name to list
     * @param parentEntry The parent entry of the directory
     * @param directoriesOnly true to list only directories, defauls to false
     *
     * @return The list of found entries
     */
    std::vector<Entry *> *listDirectory(const char *directoryName, TreeEntry **parentEntry = nullptr,
                                        const bool directoriesOnly = false);

#endif

    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     *
     * @return The partition instance
     */
    Partition *deleteFileDirectory(const TCHAR *path);

    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     *
     * @return The partition instance
     */
    Partition *deleteFileDirectory(const std::string path);

#ifndef __WIN32

    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     *
     * @return The partition instance
     */
    Partition *deleteFileDirectory(const char *path);

#endif

    /**
     * Import a single file by providing the source (file on your local disk)
     * and destination (file inside the PartitionSafe partition) paths.
     *
     * @param source The source file path on the system disk
     * @param destination The destination path on the PartitionSafe partition
     *
     * @return The partition instance
     */
    Partition *importFile(const char *source, const char *destination);

    /**
     * Export a single file by providing a source (file inside the PartitionSafe
     * partition) and destination (file on your local disk) paths.
     *
     * @param source The source file path on the PartitionSafe partition
     * @param destination The destination path on the system disk
     *
     * @return The partition instance
     */
    Partition *exportFile(const char *source, const char *destination);

};

#endif //PARTITIONSAFE_PARTITION_H
