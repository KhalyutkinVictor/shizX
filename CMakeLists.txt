cmake_minimum_required(VERSION 3.22)
project(shizX C)

set(CMAKE_C_STANDARD 17)

add_executable(shizX main.c Lexer.h Mvec.h)
