cmake_minimum_required(VERSION 3.21)
project(Win32RegCpp)

#add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall ")

#add_library(Win32RegCpp SHARED Win32RegKey.cpp Win32RegEnumeration.cpp)
add_executable(Win32RegCpp main.cpp Win32RegKey.cpp Win32RegEnumeration.cpp)