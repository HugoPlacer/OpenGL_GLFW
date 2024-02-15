cmake_minimum_required(VERSION 3.5.0)
project(Lighting-Maps VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)
enable_testing()

set(srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/Lighting-Maps.cpp
)

find_package(glfw3 3.3 REQUIRED)

add_executable(${PROJECT_NAME} ${common_files} ${srcs})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(${PROJECT_NAME} glfw)

target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_FOLDER_PATH="${CMAKE_SOURCE_DIR}/assets")
target_compile_definitions(${PROJECT_NAME} PRIVATE COMMON_SOURCE_DIR="${COMMON_SOURCE_DIR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE WINDOW_NAME="${PROJECT_NAME}")
