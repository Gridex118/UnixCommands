cmake_minimum_required(VERSION 3.10)
project(SAM C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(WC_SOURCE
    src/wc/wc.c
    )

add_executable(wc.out ${WC_SOURCE})

target_compile_options(wc.out PRIVATE -Wall -O0 -g)
