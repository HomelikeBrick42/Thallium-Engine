#include "Core/Defines.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)

#include "Platform/Win32/WindowsSurface.h"

#include "Core/Allocator.h"
#include "Core/Logger.h"

static const char* WindowClassName = "ThalliumWindowClassName";
static const DWORD WindowStyleEx = WS_EX_APPWINDOW;
static const DWORD WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

static u64 WindowCount = 0;

LRESULT CALLBACK WindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    Win32_Surface* surface = cast(Win32_Surface*) GetWindowLongPtrA(hWnd, GWLP_USERDATA);

    switch (message) {
        case WM_NCCREATE: {
            surface = (cast(CREATESTRUCTA*) lParam)->lpCreateParams;
            SetWindowLongPtrA(hWnd, GWLP_USERDATA, cast(LONG_PTR) surface);
            goto Default;
        } break;

        case WM_CLOSE:
        case WM_DESTROY: {
            if (surface->Surface->OnCloseCallback != nil) {
                surface->Surface->OnCloseCallback(surface->Surface);
            }
        } break;

        case WM_SIZE: {
            if (surface->Surface->OnResizeCallback != nil) {
                RECT clientRect;
                GetClientRect(surface->Handle, &clientRect);

                s32 width = clientRect.right - clientRect.left;
                s32 height = clientRect.bottom - clientRect.top;

                if (width > 0 && height > 0) {
                    surface->Surface->OnResizeCallback(surface->Surface, cast(u32) width, cast(u32) height);
                }
            }
        } break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            if (surface->Surface->OnKeyCallback != nil) {
                b8 pressed = (message == WM_KEYDOWN) || (message == WM_SYSKEYDOWN);

                KeyCode key = KeyCode_Unknown;

                #define KEY(name, value) case value: { key = KeyCode_ ## name; } break;
                switch (wParam) {
                    KEY(0, 0x30);
                    KEY(1, 0x31);
                    KEY(2, 0x32);
                    KEY(3, 0x33);
                    KEY(4, 0x34);
                    KEY(5, 0x35);
                    KEY(6, 0x36);
                    KEY(7, 0x37);
                    KEY(8, 0x38);
                    KEY(9, 0x39);

                    KEY(A, 0x41);
                    KEY(B, 0x42);
                    KEY(C, 0x43);
                    KEY(D, 0x44);
                    KEY(E, 0x45);
                    KEY(F, 0x46);
                    KEY(G, 0x47);
                    KEY(H, 0x48);
                    KEY(I, 0x49);
                    KEY(J, 0x4A);
                    KEY(K, 0x4B);
                    KEY(L, 0x4C);
                    KEY(M, 0x4D);
                    KEY(N, 0x4E);
                    KEY(O, 0x4F);
                    KEY(P, 0x50);
                    KEY(Q, 0x51);
                    KEY(R, 0x52);
                    KEY(S, 0x53);
                    KEY(T, 0x54);
                    KEY(U, 0x55);
                    KEY(V, 0x56);
                    KEY(W, 0x57);
                    KEY(X, 0x58);
                    KEY(Y, 0x59);
                    KEY(Z, 0x5A);

                    default: {
                        key = KeyCode_Unknown;
                    } break;
                }

                for (u64 i = 0; i < (lParam & 0xFFFF); i++) {
                    surface->Surface->OnKeyCallback(surface->Surface, key, pressed);
                }
            }

            goto Default;
        } break;

        default: Default: {
            result = DefWindowProcA(hWnd, message, wParam, lParam);
        } break;
    }

    return result;
}

b8 Win32_Surface_Create(Surface* outSurface, String name, u32 width, u32 height) {
    if (outSurface == nil) {
        return FALSE;
    }

    Win32_Surface* data = Allocate(sizeof(Win32_Surface));
    if (data == nil) {
        return FALSE;
    }

    LogInfo(String_FromLiteral("Creating Win32 Surface"));

    *data = (Win32_Surface){};
    data->Surface = outSurface;

    LogDebug(String_FromLiteral("  Getting Win32 Instance"));
    data->Instance = GetModuleHandleA(nil);
    if (data->Instance == nil) goto Error;
    LogDebug(String_FromLiteral("  Got Win32 Instance"));

    *outSurface = (Surface){
        .UserData = nil,
        .OnCloseCallback = nil,
        .OnKeyCallback = nil,
        .OnResizeCallback = nil,
        ._Destroy = Win32_Surface_Destroy,
        ._Update = Win32_Surface_Update,
        ._PrivateData = data,
    };

    // TODO: Atomic check and increment for creating windows on multiple threads
    if (WindowCount == 0) {
        LogDebug(String_FromLiteral("  Creating Win32 Window Class"));
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
        }) == 0) goto Error;
        LogDebug(String_FromLiteral("  Created Win32 Window Class"));
    }
    WindowCount++;

    RECT windowRect = {};
    windowRect.left = 100;
    windowRect.right = windowRect.left + width;
    windowRect.top = 100;
    windowRect.bottom = windowRect.top + height;
    if (!AdjustWindowRectEx(&windowRect, WindowStyle, FALSE, WindowStyleEx)) goto Error;

    LogDebug(String_FromLiteral("  Creating Win32 Window"));
    char* nameCString = String_ToTempCString(name);
    data->Handle = CreateWindowExA(
        WindowStyleEx,
        WindowClassName,
        nameCString,
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
    if (data->Handle == nil) goto Error;
    LogDebug(String_FromLiteral("  Created Win32 Window"));

    LogDebug(String_FromLiteral("  Getting Win32 Device Context"));
    data->DeviceContext = GetDC(data->Handle);
    if (data->DeviceContext == nil) goto Error;
    LogDebug(String_FromLiteral("  Got Win32 Device Context"));

    ShowWindow(data->Handle, SW_SHOW);

    LogDebug(String_FromLiteral("Created Win32 Surface\n"));
    return TRUE;
Error:
    LogError(String_FromLiteral("Failed to create Win32 Surface!\n"));
    Win32_Surface_Destroy(outSurface);
    return FALSE;
}

void Win32_Surface_Destroy(Surface* surface) {
    if (surface == nil) {
        return;
    }

    LogInfo(String_FromLiteral("Destroying Win32 Surface"));

    Win32_Surface* data = surface->_PrivateData;

    if (data->Handle != nil) {
        LogDebug(String_FromLiteral("  Destroying Win32 Window"));

        if (data->DeviceContext != nil) {
            LogDebug(String_FromLiteral("    Releasing Win32 Device Context"));
            ReleaseDC(data->Handle, data->DeviceContext);
            LogDebug(String_FromLiteral("    Released Win32 Device Context"));
        }

        DestroyWindow(data->Handle);
        LogDebug(String_FromLiteral("  Destroyed Win32 Window"));
    }

    if (WindowCount > 0) {
        WindowCount--;
        if (data->Instance != nil) {
            if (WindowCount == 0) {
                LogDebug(String_FromLiteral("  Destroying Win32 Window Class"));
                UnregisterClassA(WindowClassName, data->Instance);
                LogDebug(String_FromLiteral("  Destroyed Win32 Window Class"));
            }
        }
    }

    Deallocate(data);
    *surface = (Surface){};

    LogDebug(String_FromLiteral("Destroyed Win32 Surface\n"));
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

#endif
