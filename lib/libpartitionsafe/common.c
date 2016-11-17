//extern "C" {
    #ifdef WIN32
    #include <Rpc.h>
    #else
    #include <uuid/uuid.h>

#endif

    char* newUUID() {
    #ifdef WIN32
        UUID uuid;
        UuidCreate ( &uuid );

        unsigned char * str;
        UuidToStringA ( &uuid, &str );

        std::string s( ( char* ) str );

        RpcStringFreeA ( &str );
        return s;
    #else
        uuid_t uuid;
        uuid_generate_random(uuid);
        char s[37];
        uuid_unparse(uuid, s);
    #endif
        return s;
    }
//}