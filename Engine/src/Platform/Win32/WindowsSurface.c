#include "Platform/Win32/WindowsSurface.h"

// TODO: Custom allocator
#include <stdlib.h>

static const char* WindowClassName = "ThalliumWindowClassName";
static const DWORD WindowStyleEx = WS_EX_APPWINDOW;
static const DWORD WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

static u64 WindowCount = 0;

LRESULT CALLBACK WindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    Win32_Surface* surface = cast(Win32_Surface*) GetWindowLongPtrA(hWnd, GWLP_USERDATA);

    if (surface != nil) {
        switch (message) {
            case WM_CLOSE:
            case WM_DESTROY: {
            } break;

            default: {
                result = DefWindowProcA(hWnd, message, wParam, lParam);
            } break;
        }
    } else {
        CREATESTRUCTA* createStruct = cast(CREATESTRUCTA*) lParam;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, cast(LONG_PTR) createStruct->lpCreateParams);
        result = DefWindowProcA(hWnd, message, wParam, lParam);
    }

    return result;
}

b8 Win32_Surface_Create(Surface* outSurface, const char* name, u32 width, u32 height) {
    if (outSurface == nil) {
        return FALSE;
    }

    Win32_Surface* data = malloc(sizeof(Win32_Surface));
    *data = (Win32_Surface){};
    data->Surface = outSurface;

    data->Instance = GetModuleHandleA(nil);
    if (data->Instance == nil) {
        free(data);
        return FALSE;
    }

    // TODO: Atomic check and increment for creating windows on multiple threads
    if (WindowCount == 0) {
        if (RegisterClassExA(&(WNDCLASSEXA){
            .cbSize = sizeof(WNDCLASSEXA),
            .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = WindowMessageCallback,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = data->Instance,
            .hIcon = LoadIcon(nil, IDI_APPLICATION),
            .hCursor = LoadCursor(nil, IDC_ARROW),
            .hbrBackground = nil,
            .lpszMenuName = nil,
            .lpszClassName = WindowClassName,
            .hIconSm = nil,
        }) == 0) {
            free(data);
            return FALSE;
        }
    }
    WindowCount++;

    RECT windowRect = {};
    windowRect.left = 100;
    windowRect.right = windowRect.left + width;
    windowRect.top = 100;
    windowRect.bottom = windowRect.top + height;
    if (!AdjustWindowRectEx(&windowRect, WindowStyle, FALSE, WindowStyleEx)) {
        free(data);
        return FALSE;
    }

    data->Handle = CreateWindowExA(
        WindowStyleEx,
        WindowClassName,
        name,
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nil,
        nil,
        data->Instance,
        data
    );
    if (data->Handle == nil) {
        free(data);
        return FALSE;
    }

    data->DeviceContext = GetDC(data->Handle);
    if (data->DeviceContext == nil) {
        free(data);
        return FALSE;
    }

    ShowWindow(data->Handle, SW_SHOW);

    *outSurface = (Surface){
        ._PrivateData = data,
        .Update = Win32_Surface_Update,
    };

    return TRUE;
}

void Win32_Surface_Destroy(Surface* surface) {
    if (surface == nil) {
        return;
    }

    Win32_Surface* data = surface->_PrivateData;

    if (data->Handle != nil) {
        if (data->DeviceContext != nil) {
            ReleaseDC(data->Handle, data->DeviceContext);
        }

        DestroyWindow(data->Handle);
    }

    if (WindowCount > 0) {
        WindowCount--;
        if (data->Instance != nil) {
            if (WindowCount == 0) {
                UnregisterClassA(WindowClassName, data->Instance);
            }
        }
    }

    free(data);
    *surface = (Surface){};
}

void Win32_Surface_Update(Surface* surface) {
    if (surface == nil) {
        return;
    }

    Win32_Surface* data = surface->_PrivateData;

    MSG message;
    while (PeekMessageA(&message, data->Handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}
