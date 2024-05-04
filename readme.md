# Shader Art Viewer

This project is written entirely in C with the windows.h library. \
It seeks to allow users to write their own C shaders and deploy them as DLLs

## Installation

This project only supports Windows. 

To install, either download the latest release from the releases tab on the right, \
or download main.c and compile it on your system.

## Usage

To use the program, run this command:
```shell
ShaderArt.exe [name of the program].dll
```

### Options

There are 2 command line options included in the default application

-W or -width  [ushort screen width]  sets the width  of the application to the specified value (default: 640) \
-H or -height [ushort screen height] sets the height of the application to the specified value (default: 320)

### Custom options

Dlls may also include custom options. To check for the custom options included in a DLL run:
```shell
ShaderArt.exe --help [name of the program].dll
```
That will spit out something like this:
```text
Program arguments:
a (float) [Default 1]           | Sets the 'a' value of the parabola (y=ax^2)
start (custom) [Default bottom] | Sets the position of the pixel at the start of the parabola (bottom, center, top)
```

this tells you that you can use the options --a (with a float following it) and --start (with bottom, center, or top) \
as well as their default values and what they do

## Creating custom shaders

To create your own custom shaders, download the files in the DLL folder of this repository

Edit the library.c file to make the shader do whatever you want and compile with CMake