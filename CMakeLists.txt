cmake_minimum_required(VERSION 3.12)
project(map C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "-Wall -pedantic")

add_executable(map main.c street.c map.c street.h map.h)