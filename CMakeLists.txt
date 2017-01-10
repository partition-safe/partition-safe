cmake_minimum_required(VERSION 3.6)
project(PartitionSafe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DENABLE_TESTING=OFF")

## MBEDTLS
option(ENABLE_TESTING OFF)
option(ENABLE_PROGRAMS OFF)
add_subdirectory(lib/libmbedtls)
## /MBEDTLS

# Set sources
set(SOURCE_FILES_LIB_SQLITE lib/libsqlite/sqlite3.h lib/libsqlite/sqlite3.c lib/libsqlite/sqlite3ext.h)
set(SOURCE_FILES_LIB_FAT lib/libfatfs/src/diskio.h lib/libfatfs/src/diskio.c lib/libfatfs/src/ff.h lib/libfatfs/src/ff.c lib/libfatfs/src/ffconf.h lib/libfatfs/src/integer.h lib/libfatfs/src/option/syscall.c lib/libfatfs/src/option/unicode.c)
set(SOURCE_FILES_LIB_PARTITIONSAFE lib/libpartitionsafe/Partition.h lib/libpartitionsafe/Partition.cpp lib/libpartitionsafe/Common.h lib/libpartitionsafe/Common.cpp lib/libpartitionsafe/KeyStore.cpp lib/libpartitionsafe/KeyStore.h lib/libpartitionsafe/PartitionSafe.cpp lib/libpartitionsafe/PartitionSafe.h lib/libpartitionsafe/Vault.cpp lib/libpartitionsafe/Vault.h lib/libpartitionsafe/test.cpp lib/libpartitionsafe/filesystem/Entry.cpp lib/libpartitionsafe/filesystem/Entry.h lib/libpartitionsafe/database/User.cpp lib/libpartitionsafe/database/User.h lib/libpartitionsafe/database/Key.cpp lib/libpartitionsafe/database/Key.h lib/libpartitionsafe/filesystem/TreeEntry.cpp lib/libpartitionsafe/filesystem/TreeEntry.h lib/libpartitionsafe/NotificationCentre.cpp lib/libpartitionsafe/NotificationCentre.h lib/libpartitionsafe/notification/BaseNotification.cpp lib/libpartitionsafe/notification/BaseNotification.h)
set(SOURCE_FILES ${SOURCE_FILES_LIB_SCRYPT} ${SOURCE_FILES_LIB_SQLITE} ${SOURCE_FILES_LIB_FAT} ${SOURCE_FILES_LIB_PARTITIONSAFE})

# Set options
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Compile as executable
add_executable(PartitionSafe ${SOURCE_FILES})
target_link_libraries(PartitionSafe ${CMAKE_DL_LIBS} Threads::Threads mbedtls)

# Compile as library
add_library(PartitionSafe-Lib STATIC ${SOURCE_FILES})
set_target_properties(PartitionSafe-Lib PROPERTIES OUTPUT_NAME pslib)
target_link_libraries(PartitionSafe-Lib Threads::Threads mbedtls)
install(TARGETS PartitionSafe-Lib
        DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
