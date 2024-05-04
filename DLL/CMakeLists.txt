cmake_minimum_required(VERSION 3.26)
project(CustomShader C)

set(CMAKE_C_STANDARD 11)

add_library(CustomShader SHARED library.c
        base.h
        colours.h)
