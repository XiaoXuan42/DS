#pragma once

#include "../constant/ds_prime.h"
#include "../standard/ds_numeric_def.h"

namespace DS
{
    template<typename Key>
    struct hash {};

    inline ds_hash_type __ds_hash_string(const char *s) {
        ds_hash_type res = 0;
        for(; *s; ++s) {
            res = 13 * res + *s;
        }
        return res;
    }
    template<>
    struct hash<char*>
    {
        ds_hash_type operator()(const char * s) const { return __ds_hash_string(s); }
    };
    template<>
    struct hash<const char*>
    {
        ds_hash_type operator()(const char * s) const { return __ds_hash_string(s); }
    };

    #define hash_same_def(type_name) template<> struct hash<type_name> {\
        ds_hash_type operator() (const type_name s) const { return s; } \
    }; \
    template<> struct hash<const type_name> {\
        ds_hash_type operator() (const type_name s) const { return s; } \
    };

    hash_same_def(char);
    hash_same_def(short);
    hash_same_def(int);
    hash_same_def(long);
    hash_same_def(long long);

    #undef hash_same_def
}