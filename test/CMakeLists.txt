file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_TEST} ${SOURCE_FILES})

find_package(Threads REQUIRED)

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)


target_include_directories(${PROJECT_TEST} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_TEST} PRIVATE
    ${PROJECT_LIB}
    gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST}
    DISCOVERY_TIMEOUT 60
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
)