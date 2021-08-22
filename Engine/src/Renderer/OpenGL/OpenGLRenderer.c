#define KEEP_OPENGL_FUNCTION_MACROS
#include "Renderer/OpenGL/OpenGLRenderer.h"

#include "Core/Allocator.h"
#include "Core/Logger.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)
static void* GetOpenGLFunction(OpenGLRenderer* data, const char* name) {
    void* func = data->wglGetProcAddress(name);
    if (func == nil || func == cast(void*) 1 || func == cast(void*) 2 || func == cast(void*) 3 || func == cast(void*) -1) {
        func = GetProcAddress(data->OpenGLDllHandle, name);
    }
    return func;
}

static b8 MakeCurrent(OpenGLRenderer* data) {
    return data->wglMakeCurrent(
        (cast(Win32_Surface*) data->Renderer->_Surface->_PrivateData)->DeviceContext,
        data->OpenGLContext
    ) != FALSE;
}
#endif

static void OpenGLMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam)
{
    (void)source;
    (void)type;
    (void)id;
    (void)length;
    (void)userParam;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: {
            LogError(String_FromLiteral("OpenGL Message: %s"), String_FromCString(message));
        } break;

        case GL_DEBUG_SEVERITY_MEDIUM: {
            LogWarn(String_FromLiteral("OpenGL Message: %s"), String_FromCString(message));
        } break;

        case GL_DEBUG_SEVERITY_LOW: {
            LogInfo(String_FromLiteral("OpenGL Message: %s"), String_FromCString(message));
        } break;

        case GL_DEBUG_SEVERITY_NOTIFICATION: {
            LogDebug(String_FromLiteral("OpenGL Message: %s"), String_FromCString(message));
        } break;

        default: {
            LogFatal(String_FromLiteral("OpenGL Message: %s"), String_FromCString(message));
        } break;
    }
}

b8 OpenGLRenderer_Create(Renderer* outRenderer, Surface* surface, String name) {
    (void)name;

    LogInfo(String_FromLiteral("Creating OpenGL Renderer"));

    if (outRenderer == nil || surface == nil) {
        LogError(String_FromLiteral("Failed to create OpenGL Renderer!\n"));
        return FALSE;
    }

    OpenGLRenderer* data = Allocate(sizeof(OpenGLRenderer));
    if (data == nil) {
        LogError(String_FromLiteral("Failed to create OpenGL Renderer!\n"));
        return FALSE;
    }

    data->Renderer = outRenderer;

    *outRenderer = (Renderer){
        ._Destroy = OpenGLRenderer_Destroy,
        ._OnSurfaceResize = OpenGLRenderer_OnSurfaceResize,
        ._BeginFrame = OpenGLRenderer_BeginFrame,
        ._EndFrame = OpenGLRenderer_EndFrame,
        ._Surface = surface,
        ._PrivateData = data,
    };

#if defined(THALLIUM_PLATFORM_WINDOWS)
    LogDebug(String_FromLiteral("  Loading 'OpenGL32.dll'"));
    data->OpenGLDllHandle = LoadLibraryA("OpenGL32.dll");
    if (data->OpenGLDllHandle == nil) goto Error;
    LogDebug(String_FromLiteral("  Loaded 'OpenGL32.dll'"));
    
    LogDebug(String_FromLiteral("  Loading WGL Functions"));
    #define WGL_FUNCTION(name, ret, ...) data->name = cast(__typeof__(data->name)) GetProcAddress(data->OpenGLDllHandle, #name); if (data->name == nil) { LogError(String_FromLiteral("    Failed to load '" #name "'")); goto Error; }
        WGL_FUNCTIONS
    #undef WGL_FUNCTION
    LogDebug(String_FromLiteral("  Loaded WGL Functions"));

    {
        PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
            .nSize = sizeof(pixelFormatDescriptor),
            .nVersion = 1,
            .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType = PFD_TYPE_RGBA,
            .cColorBits = 32,
            .cDepthBits = 24,
            .cStencilBits = 8,
            .iLayerType = PFD_MAIN_PLANE,
        };

        HDC deviceContext = (cast(Win32_Surface*) data->Renderer->_Surface->_PrivateData)->DeviceContext;

        LogDebug(String_FromLiteral("  Choosing Pixel Format"));
        int format = ChoosePixelFormat(
            deviceContext,
            &pixelFormatDescriptor
        );
        if (format == 0) goto Error;
        LogDebug(String_FromLiteral("  Chose Pixel Format %u"), format);

        LogDebug(String_FromLiteral("  Setting Pixel Format"));
        if (!SetPixelFormat(
            deviceContext,
            format,
            &pixelFormatDescriptor
        )) goto Error;
        LogDebug(String_FromLiteral("  Succesfuly set Pixel Format"));

        LogDebug(String_FromLiteral("  Creating OpenGL Context"));
        data->OpenGLContext = data->wglCreateContext(deviceContext);
        if (data->OpenGLContext == nil) goto Error;
        LogDebug(String_FromLiteral("  Created OpenGL Context"));
    }

    LogDebug(String_FromLiteral("  Loading OpenGL Functions"));
    if (!MakeCurrent(data)) goto Error;
#define OPENGL_FUNCTION(name, ret, ...) data->name = GetOpenGLFunction(data, #name); if (data->name == nil) { LogError(String_FromLiteral("    Failed to load '" #name "'")); goto Error; }
    OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION
    LogDebug(String_FromLiteral("  Loaded OpenGL Functions"));
#endif

#if !defined(THALLIUM_RELEASE)
    LogDebug(String_FromLiteral("  Enabling OpenGL Debug Messenger"));

    data->glEnable(GL_DEBUG_OUTPUT);
    data->glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    data->glDebugMessageCallback(OpenGLMessageCallback, data);

	data->glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nil, GL_FALSE);

    LogDebug(String_FromLiteral("  Enabled OpenGL Debug Messenger"));
#endif

    LogDebug(String_FromLiteral("Created OpenGL Renderer\n"));
    return TRUE;
Error:
    LogError(String_FromLiteral("Failed to create OpenGL Renderer!\n"));
    OpenGLRenderer_Destroy(outRenderer);
    return FALSE;
}

void OpenGLRenderer_Destroy(Renderer* renderer) {
    LogInfo(String_FromLiteral("Destroying OpenGL Renderer"));

    if (renderer == nil) {
        return;
    }

    OpenGLRenderer* data = renderer->_PrivateData;

    if (data->OpenGLContext != nil) {
        LogDebug(String_FromLiteral("  Deleting OpenGL Context"));
        data->wglMakeCurrent(nil, nil);
        data->wglDeleteContext(data->OpenGLContext);
        LogDebug(String_FromLiteral("  Deleted OpenGL Context"));
    }

#if defined(THALLIUM_PLATFORM_WINDOWS)
    LogDebug(String_FromLiteral("  Unloading 'OpenGL32.dll'"));
    FreeLibrary(data->OpenGLDllHandle);
    LogDebug(String_FromLiteral("  Unloaded 'OpenGL32.dll'"));
#endif

    Deallocate(data);

    *renderer = (Renderer){};

    LogDebug(String_FromLiteral("Destroyed OpenGL Renderer\n"));
}

void OpenGLRenderer_OnSurfaceResize(Renderer* renderer, u32 width, u32 height) {
    if (renderer == nil) {
        return;
    }

    OpenGLRenderer* data = renderer->_PrivateData;

    if (!MakeCurrent(data)) {
        LogError(String_FromLiteral("Failed to make OpenGL Context current!"));
        return;
    }

    data->glViewport(0, 0, width, height);
}

b8 OpenGLRenderer_BeginFrame(Renderer* renderer, f32 dt) {
    (void)dt;

    if (renderer == nil) {
        return FALSE;
    }

    OpenGLRenderer* data = renderer->_PrivateData;

    if (!MakeCurrent(data)) {
        LogError(String_FromLiteral("Failed to make OpenGL Context current!"));
        return FALSE;
    }

    return TRUE;
}

b8 OpenGLRenderer_EndFrame(Renderer* renderer, f32 dt) {
    (void)dt;

    if (renderer == nil) {
        return FALSE;
    }

    OpenGLRenderer* data = renderer->_PrivateData;

    if (!MakeCurrent(data)) {
        LogError(String_FromLiteral("Failed to make OpenGL Context current!"));
        return FALSE;
    }

    SwapBuffers((cast(Win32_Surface*) data->Renderer->_Surface->_PrivateData)->DeviceContext);
    return TRUE;
}
