file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/silok/*.cpp")

add_library(${PROJECT_LIB} STATIC ${SOURCE_FILES})

find_package(Threads REQUIRED)
find_package(Spdlog REQUIRED)

target_include_directories(${PROJECT_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_LIB} PUBLIC
    Threads::Threads
    spdlog::spdlog
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

