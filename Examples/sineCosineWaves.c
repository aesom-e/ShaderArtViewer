#include <math.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "base.h"
#include "colours.h"

// Place your custom arguments in this struct
struct {
    float scale;
    unsigned short scrollSpeed;
} customFlags = {0};

// Gets custom command line flags. Use --[flag] for custom flags
void getCustomFlags(LPWSTR commandLine) {
    // Default values draw below
    customFlags.scale = 2;
    customFlags.scrollSpeed = 50;

    int argc, i, tempArg = 0;
    float temp;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
    for(i=0;i<argc;i++) {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
        char* argument = (char*)malloc(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argument, bufferSize, NULL, NULL);

        // Print custom arguments
        if(strcmp("--help", argument) == 0) {
            printf("Program arguments:\n");
            printf("scale (float)  [Default 2]  | Changes the scale of the sine wave\n");
            printf("speed (ushort) [Default 50] | Changes the scroll speed (0 for no scroll)\n");
            exit(0);
        }

        // tempArg is set to a number below this switch statement
        // Inside the switch, check for what argument was last called and preform logic based on that
        switch(tempArg) {
            case 1:
                tempArg = 0;
                temp = atof(argument);
                if(temp != 0) customFlags.scale = temp;
                break;
            case 2:
                tempArg = 0;
                temp = atof(argument);
                customFlags.scrollSpeed = (unsigned short)temp;
                break;
        }

        if(strcmp(argument, "--scale") == 0) tempArg = 1;
        if(strcmp(argument, "--speed") == 0) tempArg = 2;

        free(argument);
    }
}

pixelColour shade(flags programFlags, pixel screenPixel, float t) {
    pixelColour returnColour = {0};

    pixel center = getCenter(programFlags);
    point difference = getSignedDifference(center, screenPixel);

    // Calculate and execute cutoff if scrollSpeed != 0
    int pixelCutoff = ((int)(t * (float)customFlags.scrollSpeed) % programFlags.screenWidth);
    if(screenPixel.x > pixelCutoff && customFlags.scrollSpeed != 0) return returnColour;

    // Draw the sine wave
    float sineWaveValue = ((sinf((float)difference.x / ((float)M_PI * 3 * customFlags.scale)) * (float)M_PI * 8 * customFlags.scale));
    if(approximatelyEqual(sineWaveValue, (float)difference.y, 2)) setColour(&returnColour, TEAL);

    // Draw the cosine wave
    float cosineWaveValue = ((cosf((float)difference.x / ((float)M_PI * 3 * customFlags.scale)) * (float)M_PI * 8 * customFlags.scale));
    if(approximatelyEqual(cosineWaveValue, (float)difference.y, 2)) setColour(&returnColour, ORANGE);

    return returnColour;
}