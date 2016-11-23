//
// Created by marc on 15-11-16.
//

#include "PartitionSafe.h"

void PartitionSafe::create(const char* vaultPath, const char* keyStorePath, const char label[40], const unsigned size) {
    // Try to create the vault
    Vault::create(label, size, vaultPath);
}

PartitionSafe *PartitionSafe::init(const char* vaultPath, const char* keyStorePath) {
    // Get the vault instance
    vault = Vault::init(vaultPath, keyStorePath);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::open() {
    // Open the vault
    vault->open(fs);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::writeFile(const TCHAR *fileName, const void *buff, const UINT size) {
    // Write the file
    vault->writeFile(fileName, buff, size);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::fileInfo(const TCHAR *fileName, FILINFO *fileInfo) {
    // Get file info
    vault->fileInfo(fileName, fileInfo);

    // Return myself
    return this;
}

PartitionSafe *PartitionSafe::readFile(const TCHAR *fileName, void *buff) {
    // Read the file
    vault->readFile(fileName, buff);

    // Return myself
    return this;
}
