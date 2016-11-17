#include <iostream>
#include <sys/time.h>

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
#endif

#ifdef __APPLE__
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef GUID_WINDOWS
#include <objbase.h>
#endif

#ifdef GUID_ANDROID
#include <jni.h>
#endif

const unsigned char* newUUID()
{
//#ifdef __APPLE__
//    auto newId = CFUUIDCreate(NULL);
//    auto bytes = CFUUIDGetUUIDBytes(newId);
//    CFRelease(newId);
//
//    const unsigned char byteArray[16] =
//            {
//                    bytes.byte0,
//                    bytes.byte1,
//                    bytes.byte2,
//                    bytes.byte3,
//                    bytes.byte4,
//                    bytes.byte5,
//                    bytes.byte6,
//                    bytes.byte7,
//                    bytes.byte8,
//                    bytes.byte9,
//                    bytes.byte10,
//                    bytes.byte11,
//                    bytes.byte12,
//                    bytes.byte13,
//                    bytes.byte14,
//                    bytes.byte15
//            };
//    return byteArray;
//#endif
}
