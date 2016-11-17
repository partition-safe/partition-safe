//
// Created by marc on 15-11-16.
//

#include <iostream>
#include "Partition.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    const char* path = "/tmp/marc.vault";

    // Create a partition
    char label[40] = "Marc";
    Partition partition = Partition::create(label, 1024, path);

    // Open an already created partition
    partition = Partition::open(path);

    return 0;
}