#ifndef BASE_H
#define BASE_H

typedef unsigned char colour;

typedef struct {
    // Windows expects in the order BGR not RGB
    colour blue, green, red;
} pixelColour;

typedef struct {
    char debug;
    unsigned short screenWidth;
    unsigned short screenHeight;
} flags;

typedef struct {
    unsigned short x;
    unsigned short y;
} pixel;

typedef struct {
    short x;
    short y;
} point;

int pixelColoursAreEqual(pixelColour a, pixelColour b) {
    if(a.red == b.red
    && a.green == b.green
    && a.blue == b.blue) return 1;
    return 0;
}

int pixelsAreEqual(pixel a, pixel b) {
    if(a.x == b.x && a.y == b.y) return 1;
    return 0;
}

int abs(int in) {
    if(in < 0) return -in;
    return in;
}

float floatAbs(float in) {
    if(in < 0) return -in;
    return in;
}

int approximatelyEqual(float a, float b, float allowedVariance) {
    if(floatAbs(a - b) <= allowedVariance) return 1;
    return 0;
}

int pixelDifference(pixel a, pixel b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

pixel getCenter(flags programFlags) {
    // -15 because of the header in the application that obfuscates the top 30 pixels
    return (pixel){(programFlags.screenWidth / 2), ((programFlags.screenHeight / 2) - 15)};
}

pixel getDifference(pixel from, pixel to) {
    return (pixel){abs(from.x - to.x), abs(from.y - to.y)};
}

point getSignedDifference(pixel from, pixel to) {
    return (point){(short)(from.x - to.x), (short)(from.y - to.y)};
}

#endif
