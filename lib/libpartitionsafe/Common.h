
#ifndef PARTITIONSAFE_COMMON_H
#define PARTITIONSAFE_COMMON_H

#include <string>
#include "../libfatfs/src/ff.h"

#define KEY_SIZE 2048
#define EXPONENT 65537

class Common {
public:
    /**
     * Create a new UUID.
     *
     * @return The generated UUID.
     */
    static std::string newUUID();

    /**
     * Convert a std string to a tchar array.
     *
     * @param str The string to convert
     *
     * @return
     */
    static TCHAR* stdStringToTChar(std::string str);

    /**
     * Convert a tchar array to string.
     *
     * @param chars The array to convert
     *
     * @return
     */
    static std::string tCharToStdString(const TCHAR *chars, const UINT size = 256);

    /**
     * Create a new key pair.
     *
     * @param pubKey
     * @param privKey
     */
    static void createKeyPair(char *pubKey, char *privKey, const char *pers);

};

#endif //PARTITIONSAFE_COMMON_H
