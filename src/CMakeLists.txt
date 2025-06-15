find_package(Threads REQUIRED)
find_package(Spdlog REQUIRED)
find_package(ODB REQUIRED)

# ======================
# ✅ ODB 자동 코드 생성
# ======================
set(ODB_COMPILER "odb")
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")

# 1. 모델 파일 수집
file(GLOB_RECURSE MODEL_FILES "${CMAKE_SOURCE_DIR}/models/*.hpp")
# MODEL_FILES 수집 후 필터링
# list(FILTER MODEL_FILES EXCLUDE REGEX ".*/model.hpp$")
# 모델 이름 추출
set(MODELS "")
foreach(file IN LISTS MODEL_FILES)
  get_filename_component(model_name "${file}" NAME_WE)
  list(APPEND MODELS ${model_name})
endforeach()


# 2. 생성된 파일 저장 디렉토리
file(MAKE_DIRECTORY ${GENERATED_DIR})

foreach(model IN LISTS MODELS)
  message(STATUS "Processing model: ${model}")
  set(INPUT "${CMAKE_SOURCE_DIR}/models/${model}.hpp")
  set(COPIED_HPP "${CMAKE_BINARY_DIR}/generated/${model}.hpp")
  set(OUTPUT_CPP "${CMAKE_BINARY_DIR}/generated/${model}-odb.cxx")
  set(OUTPUT_HPP "${CMAKE_BINARY_DIR}/generated/${model}-odb.hxx")

  if (${model} STREQUAL "User")
    set(EXTRA_FLAGS "--generate-prepared")
  else()
    set(EXTRA_FLAGS "")
  endif()

  add_custom_command(
    OUTPUT ${COPIED_HPP}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${INPUT} ${COPIED_HPP}
    DEPENDS ${INPUT}
    COMMENT "Copying ${model}.hpp to generated directory"
    VERBATIM
  )
  add_custom_command(
    OUTPUT ${OUTPUT_CPP} ${OUTPUT_HPP}
    COMMAND ${ODB_COMPILER}
      -d sqlite --std c++20 --generate-query --generate-schema ${EXTRA_FLAGS}
      -o ${CMAKE_BINARY_DIR}/generated
      -I ${ODB_INCLUDE_DIR}
      ${INPUT}
    DEPENDS ${INPUT}
    COMMENT "Running ODB compiler for ${model}.hpp"
    VERBATIM
  )

  list(APPEND GENERATED_SOURCES ${OUTPUT_CPP} ${COPIED_HPP} ${OUTPUT_HPP})
endforeach()

# Add target that depends on all generated sources
add_custom_target(generate_odb ALL DEPENDS ${GENERATED_SOURCES})

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/silok/*.cpp")

add_library(${PROJECT_LIB} STATIC ${SOURCE_FILES} ${GENERATED_SOURCES})

# Include 디렉토리 (생성된 파일 포함)
target_include_directories(${PROJECT_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty/sqlite3
    ${CMAKE_SOURCE_DIR}/thirdparty/crow/include
    ${CMAKE_BINARY_DIR}/generated  # 👈 ODB가 생성한 .hxx 포함 필요
    ${ODB_INCLUDE_DIR}  # ODB 헤더 파일 경로 추가
)

target_link_libraries(${PROJECT_LIB} PUBLIC
    Threads::Threads
    spdlog::spdlog
    sqlite3
    ${ODB_LIBRARY}
    ${ODB_SQLITE_LIBRARY}
)
add_dependencies(${PROJECT_LIB} generate_odb)

# ---------------------------------------
# ✅ 메인 타겟 설정
# ---------------------------------------
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${PROJECT_LIB} 
)
