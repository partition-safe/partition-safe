#include "Common.h"
#include <iostream>
#include <sstream>

#if defined(_WIN32)
#include <windows.h>
#include <sstream>
#else
#include "../libmbedtls/include/mbedtls/entropy.h"
#include "../libmbedtls/include/mbedtls/ctr_drbg.h"
#include "../libmbedtls/include/mbedtls/rsa.h"

#endif

std::string Common::newUUID()
{
//#if defined(_WIN32)
//    SYSTEMTIME time;
//    GetSystemTime(&time);
//    WORD millis = (time.wSecond * 1000) + time.wMilliseconds;
//    std::stringStream stream;
//    stream << millis;
//    return stream.str();
//#else
//    struct timeval tp;
//    gettimeofday(&tp, NULL);
//    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
//    return  std::to_string(ms);
//#endif
    return "Yay";
}

TCHAR* Common::stdStringToTChar(std::string str) {
    // Create the TCHAR
    TCHAR *param = new TCHAR[str.size() + 1];
    param[str.size()] = 0;

    // Copy string content into TCHAR
    std::copy(str.begin(), str.end(), param);

    // Return the TCHAR
    return param;
}

std::string Common::tCharToStdString(const TCHAR *chars, const UINT size) {
    // Create string stream
    std::stringstream ss;

    // Iterate over chars
    for(int i = 0; i <= size; i++) {
        // Null byte?
        if(chars[i] == 0x00) {
            break;
        } else {
            ss << (char) chars[i];
        }
    }

    // Return the result
    return ss.str();
}

void Common::createKeyPair(const char *pers, char **pubKey, char **privKey) {
    int ret;
    mbedtls_rsa_context rsa;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    // Initialize
    mbedtls_ctr_drbg_init( &ctr_drbg );

    // Setup entropy (feed random generator)
    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                                       (const unsigned char *) pers, strlen( pers ) ) ) != 0 )
    {
        throw "Could not create seed: " + ret;
    }

    // Create RSA key
    mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
    if( ( ret = mbedtls_rsa_gen_key( &rsa, mbedtls_ctr_drbg_random, &ctr_drbg, KEY_SIZE, EXPONENT ) ) != 0 )
    {
        throw "Could not create key: " + ret;
    }

    // Write pub key
    size_t length;
    *pubKey = (char *)malloc(512 + 6 + 1);
    char *t = new char[1024];
    mbedtls_mpi_write_string(&rsa.N, 16, t, 1024, &length); // 512
    strcpy(*pubKey, t);
    mbedtls_mpi_write_string(&rsa.E, 16, t, 1024, &length); // 6
    strcat(*pubKey, t);

    // Write priv key
    *privKey = (char *)malloc(512 + 6 + 512 + (256 * 5) + 1);
    mbedtls_mpi_write_string(&rsa.N, 16, t, 1024, &length); // 512
    strcpy(*privKey, t);
    mbedtls_mpi_write_string(&rsa.E, 16, t, 1024, &length); // 6
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.D, 16, t, 1024, &length); // 512
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.P, 16, t, 1024, &length); // 256
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.Q, 16, t, 1024, &length); // 256
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.DP, 16, t, 1024, &length); // 256
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.DQ, 16, t, 1024, &length); // 256
    strcat(*privKey, t);
    mbedtls_mpi_write_string(&rsa.QP, 16, t, 1024, &length); // 256
    strcat(*privKey, t);

    // Freeup some space
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    mbedtls_rsa_free(&rsa);
}
