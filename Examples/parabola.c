#include <math.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "base.h"
#include "colours.h"

struct {
    float a;
    char useTimeForA;
    char startPos; // 0 = bottom, 1 = center, 2 = top
} customFlags = {0};

// Gets custom command line flags. Use --[flag] for custom flags
void getCustomFlags(LPWSTR commandLine) {
    customFlags.a = 1;

    int argc, i, tempArg = 0;
    float temp;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
    for(i=0;i<argc;i++) {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
        char* argument = (char*)malloc(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argument, bufferSize, NULL, NULL);

        if(strcmp("--help", argument) == 0) {
            printf("Program arguments:\n");
            printf("a (float) [Default 1]           | Sets the 'a' value of the parabola (y=ax^2)\n");
            printf("start (custom) [Default bottom] | Sets the position of the pixel at the start of the parabola (bottom, center, top)\n");
            exit(0);
        }

        switch(tempArg) {
            case 1:
                tempArg = 0;
                if(strcmp(argument, "t") == 0) { customFlags.useTimeForA = 1; break; }
                temp = (float)atof(argument);
                if(temp != 0) customFlags.a = temp;
                break;
            case 2:
                tempArg = 0;
                if(strcmp(argument, "bottom") == 0) customFlags.startPos = 0;
                if(strcmp(argument, "center") == 0) customFlags.startPos = 1;
                if(strcmp(argument, "top") == 0)    customFlags.startPos = 2;
                break;
        }

        if(strcmp("--a", argument) == 0) tempArg = 1;
        if(strcmp("--start", argument) == 0) tempArg = 2;

        free(argument);
    }
}

pixelColour shade(flags programFlags, pixel screenPixel, float t) {
    pixelColour returnColour = {0};

    pixel rootPixel;

    switch(customFlags.startPos) {
        case 1:
            rootPixel = getCenter(programFlags);
            break;
        case 2:
            rootPixel.x = getCenter(programFlags).x;
            rootPixel.y = 0;
            break;
        default:
            rootPixel.x = getCenter(programFlags).x;
            rootPixel.y = programFlags.screenHeight-30; // -30 because that's the height of the header
            break;
    }

    // Float to match a and so I don't have to cast everything in the if
    float x = (float)(rootPixel.x - screenPixel.x), y = (float)(rootPixel.y - screenPixel.y);

    float zoom = 40;
    float a = customFlags.useTimeForA ? t : customFlags.a;

    // Fully made up formula
    float thickness = ((a + zoom) * 2) / sqrtf(a + zoom);

    if(floatAbs(y - ((a * x * x)) / zoom) < thickness) setColour(&returnColour, WHITE);

    return returnColour;
}
