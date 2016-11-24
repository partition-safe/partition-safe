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
        ps->create(vaultPath, keyStorePath, label, 1024);
        std::cout << "-- Partition created" << std::endl;

        //
        // Open vault
        //

        // Init the vault
        ps->init(vaultPath, keyStorePath)->open();
        std::cout << "-- Partition opened" << std::endl;

        //
        // Write file
        //

        // File content
        const std::string filename = "sample.txt";
        const char line[] = "Hello world\nHai";

        // Write content
        ps->writeFile(filename, line, sizeof(line));
        std::cout << "-- File written" << std::endl;

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
        ps->readFile(filename, readLines, sizeof(readLines));
        std::cout << "-- Read from file: " << std::endl << readLines << std::endl;
    } catch(const char* exception) {
        // Hey, exception
        std::cout << "Thrown exception: " << exception << std::endl;
    }

    return 0;
}
