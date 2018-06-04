cmake_minimum_required(VERSION 3.7)
project(libdex)

set(CMAKE_INSTALL_PREFIX "/")

set(CMAKE_CXX_STANDARD 17)

foreach(i IN ITEMS
        -Wall
        -Wextra
        -fmax-errors=1
        -O2
        )
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:${i}>)
endforeach(i)

set(LIBDEX_SRC
        src/capi.cpp
        src/analyze.cpp
        src/dex.cpp
        src/inst.cpp
        src/insttype.cpp
        src/reader.cpp
        src/ustring.cpp
        src/ustring_encoding.cpp
        src/repackage.cpp
        )

add_library(dex SHARED ${LIBDEX_SRC})
target_include_directories(dex INTERFACE src)

install(TARGETS dex DESTINATION ./)
install(FILES dex.py DESTINATION ./)

add_executable(libdex_test ${LIBDEX_SRC} main.cpp)

add_subdirectory(tools)
