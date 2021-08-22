#pragma once

#include "Renderer/Renderer.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>
    #include "Platform/Win32/WindowsSurface.h"
#endif

#define WGL_FUNCTIONS \
    WGL_FUNCTION(wglCreateContext, HGLRC, HDC) \
    WGL_FUNCTION(wglDeleteContext, BOOL, HGLRC) \
    WGL_FUNCTION(wglMakeCurrent, BOOL, HDC, HGLRC) \
    WGL_FUNCTION(wglGetProcAddress, PROC, LPCSTR)

typedef char GLchar;
typedef b8 GLboolean;
typedef s32 GLint;
typedef u32 GLuint;
typedef u32 GLsizei;
typedef u32 GLenum;

typedef void (APIENTRY *DEBUGPROC)(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam);

#define GL_FALSE 0
#define GL_TRUE 1

#define GL_DONT_CARE 0x1100

#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B

#define OPENGL_FUNCTIONS \
    OPENGL_FUNCTION(glEnable, void, GLenum cap) \
    OPENGL_FUNCTION(glViewport, void, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION(glDebugMessageCallback, void, DEBUGPROC callback, const void * userParam) \
    OPENGL_FUNCTION(glDebugMessageControl, void, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)

typedef struct OpenGLRenderer {
    Renderer* Renderer;

#define OPENGL_FUNCTION(name, ret, ...) ret (*name)(__VA_ARGS__);
    OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #define WGL_FUNCTION(name, ret, ...) ret (__stdcall *name)(__VA_ARGS__);
        WGL_FUNCTIONS
    #undef WGL_FUNCTION

    void* OpenGLDllHandle;
    HGLRC OpenGLContext;
#endif
} OpenGLRenderer;

b8 OpenGLRenderer_Create(Renderer* outRenderer, Surface* surface, String name);
void OpenGLRenderer_Destroy(Renderer* renderer);

void OpenGLRenderer_OnSurfaceResize(Renderer* renderer, u32 width, u32 height);
b8 OpenGLRenderer_BeginFrame(Renderer* renderer, f32 dt);
b8 OpenGLRenderer_EndFrame(Renderer* renderer, f32 dt);

#if !defined(KEEP_OPENGL_FUNCTION_MACROS)
    #undef WGL_FUNCTIONS
    #undef OPENGL_FUNCTIONS
#endif
