//
// Created by marc on 15-11-16.
//

#include <iostream>
#include "PartitionSafe.h"

int main() {
    std::cout << "PartitionSafe :: Test script" << std::endl << std::endl;

    try {

        //
        // Create vault
        //

        // Vault metadata
        const char *vaultPath = "/tmp/marc.vault";
        const char *keyStorePath = "/tmp/marc.keystore";
        char label[40] = "Marc";

        // Create the partition safe instance
        PartitionSafe *ps = new PartitionSafe();

        // Create the vault
        std::cout << "-- Partition create" << std::endl;
        ps->create(vaultPath, keyStorePath, label, 1024);

        //
        // Open vault
        //

        // Init the vault
        std::cout << "-- Partition open" << std::endl;
        ps->init(vaultPath, keyStorePath)->open();

        //
        // Write file
        //

        // File content
        const std::string filename = "sample.txt";
        const char line[] = "Hello world\nHai";

        // Write content
        std::cout << "-- File write" << std::endl;
        ps->writeFile(filename, line, sizeof(line));

        //
        // Open file
        //

        // File metadata
        FILINFO fileInfo;

        // Get file info
        ps->fileInfo(filename, &fileInfo);

        // The file buffer
        char readLines[fileInfo.fsize];

        // Read content
        std::cout << "-- Read from file: " << std::endl;
        ps->readFile(filename, readLines, sizeof(readLines));
        std::cout << readLines << std::endl;
    } catch(const char* exception) {
        // Hey, exception
        std::cout << "Thrown exception: " << exception << std::endl;
    }

    return 0;
}
