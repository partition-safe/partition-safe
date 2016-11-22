cmake_minimum_required(VERSION 3.6)
project(PartitionSafe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_LIB_FAT lib/libfatfs/src/diskio.h lib/libfatfs/src/diskio.c lib/libfatfs/src/ff.h lib/libfatfs/src/ff.c lib/libfatfs/src/ffconf.h lib/libfatfs/src/integer.h lib/libfatfs/src/option/syscall.c lib/libfatfs/src/option/unicode.c)
set(SOURCE_FILES_LIB_PARTITIONSAFE lib/libpartitionsafe/Partition.h lib/libpartitionsafe/Partition.cpp lib/libpartitionsafe/common.cpp lib/libpartitionsafe/KeyStore.cpp lib/libpartitionsafe/KeyStore.h lib/libpartitionsafe/PartitionSafe.cpp lib/libpartitionsafe/PartitionSafe.h lib/libpartitionsafe/Vault.cpp lib/libpartitionsafe/Vault.h lib/libpartitionsafe/test.cpp)

set(SOURCE_FILES ${SOURCE_FILES_LIB_FAT} ${SOURCE_FILES_LIB_PARTITIONSAFE})
add_executable(PartitionSafe ${SOURCE_FILES})