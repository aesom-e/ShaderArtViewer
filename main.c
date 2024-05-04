#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "base.h"

SYSTEMTIME startTime;

typedef struct {
    char debug;
    unsigned short screenWidth;
    unsigned short screenHeight;
} flags;

flags inputFlags = {0};
char* fileName;

// DLL stuff
HINSTANCE hDLL;
typedef pixelColour (*ShadeFunction)(flags, pixel, float);
typedef void (*GetCustomFlagsFunction)(LPWSTR);
ShadeFunction shadeFunction;
GetCustomFlagsFunction getCustomFlagsFunction;

void getFlags(LPWSTR commandLine) {

    // Set default (non-zero) arguments
    inputFlags.screenWidth = 640;
    inputFlags.screenHeight = 320;

    // Get custom arguments
    int argc, i, tempArg = 0, temp;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
    for(i=1;i<argc;i++) {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
        char* argument = (char*)malloc(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argument, bufferSize, NULL, NULL);

        switch(tempArg) {
            case 1:
                tempArg = 0;
                temp = atoi(argument);
                if(temp > 0) inputFlags.screenWidth = temp;
                break;
            case 2:
                tempArg = 0;
                temp = atoi(argument);
                if(temp > 0) inputFlags.screenHeight = temp;
                break;
        }

        if(strcmp(argument, "-D") == 0)                                     inputFlags.debug = 1;
        if(strcmp(argument, "-W") == 0 || strcmp(argument, "-width") == 0)  tempArg = 1;
        if(strcmp(argument, "-H") == 0 || strcmp(argument, "-height") == 0) tempArg = 2;

        free(argument);
    }
}

float getElapsedTime() {
    SYSTEMTIME currentTime;
    GetSystemTime(&currentTime);

    // Calculate the elapsed time in seconds
    float elapsedTime = ((float) (currentTime.wSecond - startTime.wSecond) +
                         (float) (currentTime.wMinute - startTime.wMinute) * 60 +
                         (float) (currentTime.wHour - startTime.wHour) * 3600) +
                         (float) (currentTime.wMilliseconds - startTime.wMilliseconds) / 1000.0f;

    return elapsedTime;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Windows' main function. Equivalent to main() on normal C
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    GetSystemTime(&startTime);

    LPWSTR commandLine = GetCommandLineW();

    int argc;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);

    if(argc < 2) {
        MessageBox(NULL, "A DLL file must be provided", "Input", MB_ICONERROR);
        exit(0);
    }

    getFlags(commandLine);

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, argv[argc-1], -1, NULL, 0, NULL, NULL);
    fileName = (char*)malloc(bufferSize);
    WideCharToMultiByte(CP_UTF8, 0, argv[argc-1], -1, fileName, bufferSize, NULL, NULL);

    hDLL = LoadLibrary(fileName);
    if(hDLL == NULL) {
        MessageBox(NULL, "DLL failed to load", "Error", MB_ICONERROR);
        exit(-1);
    }
    shadeFunction = (ShadeFunction)GetProcAddress(hDLL, "shade");
    if(shadeFunction == NULL) {
        MessageBox(NULL, "Function \"shade(flags, pixel, float)\" failed to load", "Error", MB_ICONERROR);
        exit(-1);
    }
    getCustomFlagsFunction = (GetCustomFlagsFunction)GetProcAddress(hDLL, "getCustomFlags");
    if(shadeFunction == NULL) {
        MessageBox(NULL, "Function \"getCustomFlags(LPWSTR)\" failed to load", "Error", MB_ICONERROR);
        exit(-1);
    }

    getCustomFlagsFunction(commandLine);

    // Register the window class
    const char CLASS_NAME[] = "Shader Render";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Shader Render at t=0.0s",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, inputFlags.screenWidth, inputFlags.screenHeight,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    if(hwnd == NULL) {
        return 0;
    }

    SetTimer(hwnd, 1, 1000 / 30, NULL);

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_TIMER:
            InvalidateRect(hwnd, NULL, FALSE); // Invalidate the entire window to trigger a repaint
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Create a compatible memory device context
            HDC memDC = CreateCompatibleDC(hdc);
            if(memDC == NULL) {
                EndPaint(hwnd, &ps);
                return 0;
            }

            // Create a bitmap compatible with the window's DC
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, inputFlags.screenWidth, inputFlags.screenHeight);
            if(hBitmap == NULL) {
                DeleteDC(memDC);
                EndPaint(hwnd, &ps);
                return 0;
            }

            // Select the bitmap into the memory DC
            HBITMAP hBitmapOld = (HBITMAP)SelectObject(memDC, hBitmap);

            // Allocate memory for the pixel data
            pixelColour* pixelData = (pixelColour*)malloc(inputFlags.screenWidth * inputFlags.screenHeight * sizeof(pixelColour));

            float elapsedTime = getElapsedTime();
            int x, y;

            for(y = 0; y < inputFlags.screenHeight; y++) {
                for(x = 0; x < inputFlags.screenWidth; x++) {
                    // Calculate the index in the pixel data array
                    int index = y * inputFlags.screenWidth + x;
                    pixel inputPixel = {x, y};
                    pixelData[index] = shadeFunction(inputFlags, inputPixel, elapsedTime);
                }
            }

            BITMAPINFO bmi;
            memset(&bmi, 0, sizeof(BITMAPINFO));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = inputFlags.screenWidth;
            bmi.bmiHeader.biHeight = -inputFlags.screenHeight; // Negative height to indicate top-down bitmap
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 24; // 24 bits per pixel (RGB)
            bmi.bmiHeader.biCompression = BI_RGB;

            // Set the pixel data to the bitmap
            SetDIBits(memDC, hBitmap, 0, inputFlags.screenHeight, pixelData, &bmi, DIB_RGB_COLORS);

            // Bit block transfer from memory DC to window DC
            BitBlt(hdc, 0, 0, inputFlags.screenWidth, inputFlags.screenHeight, memDC, 0, 0, SRCCOPY);

            // Cleanup
            free(pixelData);
            SelectObject(memDC, hBitmapOld);
            DeleteObject(hBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);

            float frameTime = getElapsedTime() - elapsedTime;
            char windowTitle[100];
            snprintf(windowTitle, sizeof(windowTitle), "Shader Render at t=%.1fs (FPS: %.1f)", elapsedTime, 1 / frameTime);

            // Update the window title
            SetWindowText(hwnd, windowTitle);
        }
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
