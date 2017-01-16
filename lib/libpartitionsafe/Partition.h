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
    static const unsigned char *IDENTIFIER;

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
     * Partition destructor.
     */
    ~Partition();

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
     * Write to a file on the partition.
     *
     * @param fileName
     * @param buff
     * @param size
     *
     * @return
     */
    Partition* writeFile(const std::string fileName, const void* buff, const UINT size);

    /**
 * Rename a file or folder.
 *
 * @param oldPath - Path to the file or folder to be renamed
 * @param newPath - Path to the file or folder with changed name
 * @return
 */
    Partition *renameFileFolder(const char *oldPath, const char *newPath);

#ifndef __WIN32
    /**
     * Write to a file on the partition.
     *
     * @param fileName
     * @param buff
     * @param size
     *
     * @return
     */
    Partition* writeFile(const char *fileName, const void* buff, const UINT size);
#endif

    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    Partition* fileInfo(const TCHAR *fileName, FILINFO *fileInfo);

    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    Partition* fileInfo(const std::string fileName, FILINFO *fileInfo);

#ifndef __WIN32
    /**
     * Get the size a the file on the partition.
     *
     * @param fileName
     *
     * @return
     */
    Partition* fileInfo(const char *fileName, FILINFO *fileInfo);
#endif

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
     * @param fileName
     * @param buff
     *
     * @return
     */
    Partition* readFile(const std::string fileName, void *buff, const UINT size);

#ifndef __WIN32
    /**
     * Read the contents of a file on the partition.
     *
     * @param fileName
     * @param buff
     *
     * @return
     */
    Partition* readFile(const char *fileName, void *buff, const UINT size);
#endif

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

#ifndef __WIN32
    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return
     */
    Partition* createDirectory(const char *directoryName);
#endif

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const TCHAR *directoryName, TreeEntry **parentEntry = nullptr, const bool directoriesOnly = false);

    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const std::string directoryName, TreeEntry **parentEntry = nullptr, const bool directoriesOnly = false);

#ifndef __WIN32
    /**
     * Read the contents of a file on the partition.
     *
     * @param directoryName
     *
     * @return The list of found entries
     */
    std::vector<Entry*> *listDirectory(const char *directoryName, TreeEntry **parentEntry = nullptr, const bool directoriesOnly = false);
#endif

    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     */
    Partition *deleteFileDirectory(const TCHAR *path);

    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     */
    Partition *deleteFileDirectory(const std::string path);

#ifndef __WIN32
    /**
     * Delete the given file or directory.
     *
     * @param path The directory to delete
     */
    Partition *deleteFileDirectory(const char *path);
#endif

    /**
     * Import a single file by providing the source (file on your local disk)
     * and destination (file inside the PartitionSafe partition) paths.
     *
     * @param source The source file path
     * @param destination The destination path
     * @return
     */
    Partition *importFile(const char* source, const char* destination);

    /**
     * Export a single file by providing a source (file inside the PartitionSafe
     * partition) and destination (file on your local disk) paths.
     *
     * @param source The source file path
     * @param destination The destination path
     * @return
     */
    Partition *exportFile(const char* source, const char* destination);

};

#endif //PARTITIONSAFE_PARTITION_H
