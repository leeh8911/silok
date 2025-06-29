find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/silok/*.cpp")

add_library(${PROJECT_LIB} STATIC ${SOURCE_FILES} ${GENERATED_SOURCES})

# Include 디렉토리 (생성된 파일 포함)
target_include_directories(${PROJECT_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    /usr/local/include
    /usr/local/include/bcrypt
)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_LIB} PUBLIC
    Threads::Threads
    spdlog::spdlog
    sqlite3
    OpenSSL::Crypto
    bcrypt
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${PROJECT_LIB} 
)
