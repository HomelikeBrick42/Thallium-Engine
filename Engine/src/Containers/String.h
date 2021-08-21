#pragma once

#include "Core/Defines.h"

typedef struct String {
    u8* Data;
    u64 Length;
} String;

#define String_FromLiteral(s) ((String){ .Data = cast(u8*) (s), .Length = sizeof(s) - 1 })

THALLIUM_INLINE String String_Create(u8* data, u64 length) {
    String result;
    result.Data = data;
    result.Length = length;
    return result;
}

THALLIUM_INLINE String String_FromCString(const char* cstring) {
    String result;
    result.Data = cast(u8*) cstring;
    result.Length = 0;

    char* ptr = cast(char*) cstring;
    while (*ptr != '\0') {
        result.Length++;
        ptr++;
    }

    return result;
}

THALLIUM_API char* String_ToTempCString(String s);
