//
// Created by marc on 15-11-16.
//

#include <iostream>
#include "PartitionSafe.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    PartitionSafe partitionSafe;

    Partition partition = partitionSafe.createPartition("Marc", 1024 * 1024, "/tmp/marc.vault");
    partition.open();

    return 0;
}