cmake_minimum_required(VERSION 3.21)
project(Win32RegCpp)

set(CMAKE_CXX_STANDARD 17)

#add_library(Win32RegCpp SHARED Win32RegKey.cpp Win32RegEnumeration.cpp)
add_executable(Win32RegCpp main.cpp Win32RegKey.cpp Win32RegEnumeration.cpp)