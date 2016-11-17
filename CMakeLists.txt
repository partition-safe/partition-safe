cmake_minimum_required(VERSION 3.6)
project(PartitionSafe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework CoreFoundation")

set(SOURCE_FILES
        lib/libpartitionsafe/Partition.cpp
        lib/libpartitionsafe/Partition.h
        lib/libpartitionsafe/PartitionSafe.cpp
        lib/libpartitionsafe/PartitionSafe.h
        lib/libpartitionsafe/Vault.cpp
        lib/libpartitionsafe/Vault.h
        lib/libpartitionsafe/KeyStore.cpp
        lib/libpartitionsafe/KeyStore.h
        lib/libpartitionsafe/test.cpp
        lib/libpartitionsafe/guid/guid.cpp
        lib/libpartitionsafe/guid/guid.h)
add_executable(PartitionSafe ${SOURCE_FILES})