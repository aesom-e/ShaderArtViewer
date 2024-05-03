#include <math.h>
#include <windows.h>
#include <string.h>
#include "base.h"
#include "colours.h"

// NOTE: EVERYTHING IN "///" IS TO SHOW A BASIC WAY TO WRITE THIS DLL

// Place your custom arguments in this struct
struct {
    /// float a;
} customFlags = {0};

// Gets custom command line flags. Use --[flag] for custom flags
void getCustomFlags(LPWSTR commandLine) {
    // Default values go below

    int argc, i, tempArg = 0;
    float temp;
    LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
    for(i=0;i<argc;i++) {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
        char* argument = (char*)malloc(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, argument, bufferSize, NULL, NULL);

        // tempArg is set to a number below this switch statement
        // Inside the switch, check for what argument was last called and preform logic based on that
        switch(tempArg) {
            /// case 1:
            ///     tempArg = 0;
            ///     temp = atof(argument);
            ///     if(temp != 0) customFlags.a = temp;
            ///     break;
        }

        /// if(strcmp(argument, "--a") == 0) tempArg = 1;

        free(argument);
    }
}

// Main function, called per pixel per frame
// screenPixel has the location of the pixel currently being rendered and t is the current time
pixelColour shade(flags programFlags, pixel screenPixel, float t) {
    pixelColour returnColour = {0};

    /// pixel center = getCenter(programFlags);
    /// if(pixelsAreEqual(center, screenPixel)) setColour(&returnColour, WHITE);

    return returnColour;
}
