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

THALLIUM_INLINE b8 String_Equal(String a, String b) {
    if (a.Length != b.Length) {
        return FALSE;
    }

    if (a.Data == b.Data) {
        return TRUE;
    }

    for (u64 i = 0; i < a.Length; i++) {
        if (a.Data[i] != b.Data[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

THALLIUM_API char* String_ToTempCString(String s);
