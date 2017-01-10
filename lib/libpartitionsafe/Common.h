
#ifndef PARTITIONSAFE_COMMON_H
#define PARTITIONSAFE_COMMON_H

#include <string>
#include "../libfatfs/src/ff.h"

#define KEY_SIZE 2048
#define EXPONENT 65537
#define RSA_PUBLIC_SIZE 512 + 6 + 1
#define RSA_PRIVATE_SIZE 512 + 6 + 512 + (256 * 5) + 1

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
    static void createKeyPair(const char *pers, char **pubKey, char **privKey);

    /**
     * Generate a random string.
     *
     * @param size
     * @param output
     */
    static void randomChars(unsigned size, char **output);

    /**
     * Generate a random string.
     *
     * @param size
     * @param output
     */
    static void randomChars(unsigned size, unsigned char **output);

};

#endif //PARTITIONSAFE_COMMON_H
