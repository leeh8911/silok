file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_TEST} ${SOURCE_FILES})

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

target_include_directories(${PROJECT_TEST} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_TEST} PRIVATE
    ${PROJECT_LIB}
    GTest::gtest_main
    GTest::gtest
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST}
    DISCOVERY_TIMEOUT 60
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
)
