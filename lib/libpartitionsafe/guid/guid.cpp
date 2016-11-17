//
// Created by Youri Lefers on 17-11-16.
//

#include "guid.h"

#if defined(__linux__) || defined(__unix__)
#include <uuid/uuid.h>
#endif

#ifdef __APPLE__
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef _WIN32
#include <objbase.h>
#endif

using namespace std;

// overload << so that it's easy to convert to a string
ostream &operator<<(ostream &s, const Guid &guid)
{
    return s << hex << setfill('0')
             << setw(2) << (int)guid._bytes[0]
             << setw(2) << (int)guid._bytes[1]
             << setw(2) << (int)guid._bytes[2]
             << setw(2) << (int)guid._bytes[3]
             << "-"
             << setw(2) << (int)guid._bytes[4]
             << setw(2) << (int)guid._bytes[5]
             << "-"
             << setw(2) << (int)guid._bytes[6]
             << setw(2) << (int)guid._bytes[7]
             << "-"
             << setw(2) << (int)guid._bytes[8]
             << setw(2) << (int)guid._bytes[9]
             << "-"
             << setw(2) << (int)guid._bytes[10]
             << setw(2) << (int)guid._bytes[11]
             << setw(2) << (int)guid._bytes[12]
             << setw(2) << (int)guid._bytes[13]
             << setw(2) << (int)guid._bytes[14]
             << setw(2) << (int)guid._bytes[15];
}

// create a guid from vector of bytes
Guid::Guid(const vector<unsigned char> &bytes)
{
    _bytes = bytes;
}

// create a guid from array of bytes
Guid::Guid(const unsigned char *bytes)
{
    _bytes.assign(bytes, bytes + 16);
}

// converts a single hex char to a number (0 - 15)
unsigned char hexDigitToChar(char ch)
{
    if (ch > 47 && ch < 58)
        return ch - 48;

    if (ch > 96 && ch < 103)
        return ch - 87;

    if (ch > 64 && ch < 71)
        return ch - 55;

    return 0;
}

// converts the two hexadecimal characters to an unsigned char (a byte)
unsigned char hexPairToChar(char a, char b)
{
    return hexDigitToChar(a) * 16 + hexDigitToChar(b);
}

// create a guid from string
Guid::Guid(const string &fromString)
{
    _bytes.clear();

    char charOne, charTwo;
    bool lookingForFirstChar = true;

    for (const char &ch : fromString)
    {
        if (ch == '-')
            continue;

        if (lookingForFirstChar)
        {
            charOne = ch;
            lookingForFirstChar = false;
        }
        else
        {
            charTwo = ch;
            auto byte = hexPairToChar(charOne, charTwo);
            _bytes.push_back(byte);
            lookingForFirstChar = true;
        }
    }

}

// create empty guid
Guid::Guid()
{
    _bytes = vector<unsigned char>(16, 0);
}

// copy constructor
Guid::Guid(const Guid &other)
{
    _bytes = other._bytes;
}

// overload assignment operator
Guid &Guid::operator=(const Guid &other)
{
    _bytes = other._bytes;
    return *this;
}

// overload equality operator
bool Guid::operator==(const Guid &other) const
{
    return _bytes == other._bytes;
}

// overload inequality operator
bool Guid::operator!=(const Guid &other) const
{
    return !((*this) == other);
}

// This is the linux friendly implementation, but it could work on other
// systems that have libuuid available
#if defined(__linux__) || defined(__unix__)
Guid GuidGenerator::newGuid()
{
  uuid_t id;
  uuid_generate(id);
  return id;
}
#endif

// this is the mac and ios version
#ifdef __APPLE__
Guid GuidGenerator::newGuid()
{
  auto newId = CFUUIDCreate(NULL);
  auto bytes = CFUUIDGetUUIDBytes(newId);
  CFRelease(newId);

  const unsigned char byteArray[16] =
  {
    bytes.byte0,
    bytes.byte1,
    bytes.byte2,
    bytes.byte3,
    bytes.byte4,
    bytes.byte5,
    bytes.byte6,
    bytes.byte7,
    bytes.byte8,
    bytes.byte9,
    bytes.byte10,
    bytes.byte11,
    bytes.byte12,
    bytes.byte13,
    bytes.byte14,
    bytes.byte15
  };
  return byteArray;
}
#endif

// obviously this is the windows version
#ifdef _WIN32
Guid GuidGenerator::newGuid()
{
  GUID newId;
  CoCreateGuid(&newId);

  const unsigned char bytes[16] =
  {
    (newId.Data1 >> 24) & 0xFF,
    (newId.Data1 >> 16) & 0xFF,
    (newId.Data1 >> 8) & 0xFF,
    (newId.Data1) & 0xff,

    (newId.Data2 >> 8) & 0xFF,
    (newId.Data2) & 0xff,

    (newId.Data3 >> 8) & 0xFF,
    (newId.Data3) & 0xFF,

    newId.Data4[0],
    newId.Data4[1],
    newId.Data4[2],
    newId.Data4[3],
    newId.Data4[4],
    newId.Data4[5],
    newId.Data4[6],
    newId.Data4[7]
  };

  return bytes;
}
#endif
