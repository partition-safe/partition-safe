cmake_minimum_required(VERSION 3.6)
project(PartitionSafe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#include(${CMAKE_CURRENT_SOURCE_DIR}/lib/libmbedtls/CMakeLists.txt -DUSE_SHARED_MBEDTLS_LIBRARY:BOOL=TRUE -DENABLE_TESTING=Off)

## MBEDTLS

add_subdirectory(lib/libmbedtls)

## /MBEDTLS

set(SOURCE_FILES_LIB_SQLITE lib/libsqlite/sqlite3.h lib/libsqlite/sqlite3.c lib/libsqlite/sqlite3ext.h)
set(SOURCE_FILES_LIB_FAT lib/libfatfs/src/diskio.h lib/libfatfs/src/diskio.c lib/libfatfs/src/ff.h lib/libfatfs/src/ff.c lib/libfatfs/src/ffconf.h lib/libfatfs/src/integer.h lib/libfatfs/src/option/syscall.c lib/libfatfs/src/option/unicode.c)
set(SOURCE_FILES_LIB_PARTITIONSAFE lib/libpartitionsafe/Partition.h lib/libpartitionsafe/Partition.cpp lib/libpartitionsafe/Common.h lib/libpartitionsafe/Common.cpp lib/libpartitionsafe/KeyStore.cpp lib/libpartitionsafe/KeyStore.h lib/libpartitionsafe/PartitionSafe.cpp lib/libpartitionsafe/PartitionSafe.h lib/libpartitionsafe/Vault.cpp lib/libpartitionsafe/Vault.h lib/libpartitionsafe/test.cpp lib/libpartitionsafe/Entry.cpp lib/libpartitionsafe/Entry.h lib/libpartitionsafe/database/User.cpp lib/libpartitionsafe/database/User.h lib/libpartitionsafe/database/Key.cpp lib/libpartitionsafe/database/Key.h)

set(SOURCE_FILES ${SOURCE_FILES_LIB_SCRYPT} ${SOURCE_FILES_LIB_SQLITE} ${SOURCE_FILES_LIB_FAT} ${SOURCE_FILES_LIB_PARTITIONSAFE})
add_executable(PartitionSafe ${SOURCE_FILES})

target_link_libraries(PartitionSafe mbedtls)
