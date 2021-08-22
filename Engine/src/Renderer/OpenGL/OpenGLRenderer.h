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

typedef int GLint;
typedef unsigned int GLsizei;

#define OPENGL_FUNCTIONS \
    OPENGL_FUNCTION(glViewport, void, GLint x, GLint y, GLsizei width, GLsizei height)

typedef struct OpenGLRenderer {
    Renderer* Renderer;

#define OPENGL_FUNCTION(name, ret, ...) ret (*name)(__VA_ARGS__);
    OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #define WGL_FUNCTION(name, ret, ...) ret (*name)(__VA_ARGS__);
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
