
#ifndef PARTITIONSAFE_COMMON_H
#define PARTITIONSAFE_COMMON_H

#include <string>
#include "../libfatfs/src/ff.h"

class Common {
public:
    /**
     * Create a new UUID.
     *
     * @return The generated UUID.
     */
    static std::string newUUID();

    /**
     * Convert a std string to TCHAR.
     *
     * @param str The string to convert
     *
     * @return
     */
    static TCHAR* stdStringToTChar(std::string str);

};

#endif //PARTITIONSAFE_COMMON_H
