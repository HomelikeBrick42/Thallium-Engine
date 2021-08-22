#include "Core/Logger.h"

#include "Containers/Buffer.h"

#include <stdarg.h>
#include <stdio.h>

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>
#endif

void Logger_Log(LogLevel level, String format, ...) {
    (void)level;

    u8* buffer = Buffer_Create(u8);

    __builtin_va_list args;
    va_start(args, format);

    for (u64 i = 0; i < format.Length; i++) {
        u8 chr = format.Data[i];

        if (chr == '%') {
            i++;
            switch (format.Data[i]) {
                case '%': {
                    Buffer_Push(buffer, '%');
                } break;

                case 's': {
                    String string = va_arg(args, String);
                    for (u64 i = 0; i < string.Length; i++) {
                        Buffer_Push(buffer, string.Data[i]);
                    }
                } break;

                case 'l': {
                    i++;
                    if (format.Data[i] == 'l' && format.Data[i + 1] == 'u') {
                        i += 2;

                        u64 value = va_arg(args, u64);
                        char buf[128] = {};
                        sprintf(buf, "%llu", value);
                        String string = String_FromCString(buf);
                        for (u64 i = 0; i < string.Length; i++) {
                            Buffer_Push(buffer, string.Data[i]);
                        }
                    }
                } break;

                case 'u': {
                    u32 value = va_arg(args, u32);
                    char buf[128] = {};
                    sprintf(buf, "%u", value);
                    String string = String_FromCString(buf);
                    for (u64 i = 0; i < string.Length; i++) {
                        Buffer_Push(buffer, string.Data[i]);
                    }
                } break;

                default: {
                    Buffer_Push(buffer, '%');
                } break;
            }
        } else {
            Buffer_Push(buffer, chr);
        }
    }

    va_end(args);

    Buffer_Push(buffer, '\n');

#if defined(THALLIUM_PLATFORM_WINDOWS)
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo = {};
    GetConsoleScreenBufferInfo(console, &consoleInfo);

    WORD oldColor = consoleInfo.wAttributes;
    WORD newColor =
        (level == LogLevel_Trace) ? (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) :
        (level == LogLevel_Debug) ? (FOREGROUND_BLUE | FOREGROUND_INTENSITY) :
        (level == LogLevel_Info) ? (FOREGROUND_GREEN | FOREGROUND_INTENSITY) :
        (level == LogLevel_Warn) ? (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) :
        (level == LogLevel_Error) ? (FOREGROUND_RED | FOREGROUND_INTENSITY) :
        (level == LogLevel_Fatal) ? (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY) : 0;

    SetConsoleTextAttribute(console, newColor);
    WriteConsoleA(console, buffer, cast(DWORD) Buffer_Length(buffer), nil, nil);
    SetConsoleTextAttribute(console, oldColor);
#endif

    Buffer_Destroy(buffer);
}
