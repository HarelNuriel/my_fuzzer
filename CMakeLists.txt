cmake_minimum_required(VERSION 4.1)
project(my_fuzzer C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)

add_executable(fuzzer ${SOURCE_FILES})
