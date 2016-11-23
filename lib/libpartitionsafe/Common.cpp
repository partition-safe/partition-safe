#include "Common.h"
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include <sstream>
#else
#include <sys/time.h>
#include "../libfatfs/src/ff.h"
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
