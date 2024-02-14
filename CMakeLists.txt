cmake_minimum_required(VERSION 3.5.0)
project(OpenGL_GLFW VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)
enable_testing()

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/6.Camera/Camera.cpp"
        "${PROJECT_SOURCE_DIR}/src/6.Camera/shader_s.h"
        "${PROJECT_SOURCE_DIR}/src/glad.c"
        )

#file(GLOB SHADE
#        "${PROJECT_SOURCE_DIR}/src/Coord/shader.fs"
#        "${PROJECT_SOURCE_DIR}/src/Textures/shader.vs"
#        )

#foreach(SHADER ${SHADERS})
   #file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/build/)
#endforeach(SHADER ${SHADERS})

find_package(glfw3 3.3 REQUIRED)

#SET(GCC_COVERAGE_COMPILE_FLAGS "-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl")
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(OpenGL_GLFW ${all_SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(OpenGL_GLFW glfw)
