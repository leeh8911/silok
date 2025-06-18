cmake_minimum_required(VERSION 3.22)

project(silok)

cmake_policy(SET CMP0074 NEW)

message(STATUS ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})
message(STATUS ${CMAKE_PREFIX_PATH})

# ====================
# C++ 설정
# ====================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ====================
# 전역 변수
# ====================
set(PROJECT_LIB "lsilok")
set(PROJECT_TEST "test_silok")

set(BUILD_TEST ON CACHE BOOL "Build tests")
set(BUILD_DOCS ON CACHE BOOL "Build docs")
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)

# ====================
# 컴파일러 옵션
# ====================
# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_compile_options(/MDd)
# else()
#     add_compile_options(/MD)
# endif()

# ====================
# 모듈 경로 및 외부 패키지
# ====================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(OpenSSL REQUIRED)

# ====================
# Doxygen 설정
# ====================
find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs/doxygen")
    set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/index.html")

    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "📘 Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# ====================
# 서브 디렉토리 구성
# ====================
add_subdirectory(src)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
