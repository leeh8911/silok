cmake_minimum_required(VERSION 3.10)

project(silok)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_LIB "lsilok")
set(PROJECT_TEST "test_silok")

set(BUILD_TEST OFF CACHE BOOL "Build tests")

add_subdirectory(src)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif(BUILD_TEST)


