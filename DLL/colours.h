#include "base.h"

#ifndef COLOURS_H
#define COLOURS_H

typedef enum {
    WHITE,
    BLACK,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    TEAL,
    BLUE,
    PURPLE,
    PINK
} colourName;

void setColour(pixelColour* pixelIn, colourName name) {
    pixelIn->red = 0;
    pixelIn->green = 0;
    pixelIn->blue = 0;
    switch(name) {
        case WHITE:
            pixelIn->red = 255;
            pixelIn->green = 255;
            pixelIn->blue = 255;
            break;
        case RED:
            pixelIn->red = 255;
            break;
        case ORANGE:
            pixelIn->red = 255;
            pixelIn->green = 127;
            break;
        case YELLOW:
            pixelIn->red = 255;
            pixelIn->green = 255;
            break;
        case GREEN:
            pixelIn->green = 255;
            break;
        case TEAL:
            pixelIn->green = 191;
            pixelIn->blue = 191;
            break;
        case BLUE:
            pixelIn->blue = 255;
            break;
        case PURPLE:
            pixelIn->red = 127;
            pixelIn->blue = 255;
            break;
        case PINK:
            pixelIn->red = 255;
            pixelIn->blue = 255;
            break;
    }
}

#endif