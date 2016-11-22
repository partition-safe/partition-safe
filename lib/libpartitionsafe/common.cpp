#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#include <sstream>
#else
#include <sys/time.h>
#endif

/*
std::string newUUID();

std::string newUUID()
{
#if defined(_WIN32)
    SYSTEMTIME time;
    GetSystemTime(&time);
    WORD millis = (time.wSecond * 1000) + time.wMilliseconds;
    std::stringStream stream;
    stream << millis;
    return stream.str();
#else
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return  std::to_string(ms);
#endif
}
*/
