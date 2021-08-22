#pragma once

#include "Core/Defines.h"
#include "Containers/String.h"

typedef enum LogLevel {
    LogLevel_Trace,
    LogLevel_Debug,
    LogLevel_Info,
    LogLevel_Warn,
    LogLevel_Error,
    LogLevel_Fatal,
} LogLevel;

THALLIUM_API void Logger_Log(LogLevel level, String format, ...);

#define LogTrace(format, ...) Logger_Log(LogLevel_Trace, format, ##__VA_ARGS__)
#define LogDebug(format, ...) Logger_Log(LogLevel_Debug, format, ##__VA_ARGS__)
#define LogInfo(format, ...) Logger_Log(LogLevel_Info, format, ##__VA_ARGS__)
#define LogWarn(format, ...) Logger_Log(LogLevel_Warn, format, ##__VA_ARGS__)
#define LogError(format, ...) Logger_Log(LogLevel_Error, format, ##__VA_ARGS__)
#define LogFatal(format, ...) Logger_Log(LogLevel_Fatal, format, ##__VA_ARGS__)
