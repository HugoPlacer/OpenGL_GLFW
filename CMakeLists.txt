cmake_minimum_required(VERSION 3.5.0)
project(OpenGL_GLFW VERSION 0.1.0 LANGUAGES C CXX)

# Define common variables
set(COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(COMMON_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(GLAD_SOURCE ${COMMON_SOURCE_DIR}/glad.c)

file(GLOB common_files
        "${COMMON_INCLUDE_DIR}/*.h"
        "${COMMON_INCLUDE_DIR}/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/glad.c"
        )

add_subdirectory(${COMMON_SOURCE_DIR}/Getting-Started/5.Coord-Systems)
add_subdirectory(${COMMON_SOURCE_DIR}/Getting-Started/6.Camera)

add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/1.Colors)
add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/2.Basic-Lighting)
add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/3.Materials)
add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/4.Lighting-Maps)
add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/5.Light-Casters)
add_subdirectory(${COMMON_SOURCE_DIR}/Lighting/6.Multiple-Lights)

add_subdirectory(${COMMON_SOURCE_DIR}/Model-Loading/1.Model-Loading)

add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/1.Depth-Testing)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/2.Stencil-Testing)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/3.Blending)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/4.Face-Culling)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/5.Framebuffers)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-OpenGL/6.Cubemaps)

add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-Lighting/1.Advanced-Lighting)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-Lighting/2.Gamma-Correction)
add_subdirectory(${COMMON_SOURCE_DIR}/Advanced-Lighting/3.Shadows/Shadow-Mapping)