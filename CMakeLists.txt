cmake_minimum_required(VERSION 3.10)
project(SAM C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(WC_SOURCE
    src/wc/wc.c
    )

set(LS_SOURCE
    src/ls/ls.c
    )

add_executable(wc.out ${WC_SOURCE})
add_executable(ls.out ${LS_SOURCE})
target_link_libraries(ls.out m)

target_compile_options(wc.out PRIVATE -Wall -O0 -g)
target_compile_options(ls.out PRIVATE -Wall -Wextra -O0 -g)
