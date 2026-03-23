cmake_minimum_required(VERSION 4.1)
project(my_fuzzer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES main.c
                include/fuzz.h
                src/fuzz.h
                )

add_executable(fuzzer ${SOURCE_FILES})

target_include_directories(fuzzer PUBLIC ${CMAKE_SOURCE_DIR}/include)
