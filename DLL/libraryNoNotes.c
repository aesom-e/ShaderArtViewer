#include <math.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "base.h"
#include "colours.h"

// Place your custom arguments in this struct
struct {

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

        // Print custom arguments
        if(strcmp("--help", argument) == 0) {
            printf("No custom arguments\n");
            exit(0);
        }

        // tempArg is set to a number below this switch statement
        // Inside the switch, check for what argument was last called and preform logic based on that
        switch(tempArg) {

        }

        free(argument);
    }
}

// Main function, called per pixel per frame
// screenPixel has the location of the pixel currently being rendered and t is the current time
pixelColour shade(flags programFlags, pixel screenPixel, float t) {
    pixelColour returnColour = {0};

    return returnColour;
}
