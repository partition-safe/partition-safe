#include <iostream>
#include <sys/time.h>

std::string newUUID()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    return  std::to_string(ms);
}
