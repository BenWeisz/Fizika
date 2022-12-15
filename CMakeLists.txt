cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Fizika VERSION 1.0)

add_definitions(-std=c++17)

add_executable(Fizika main.cpp)

set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if(${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif()

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(Fizika PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(Fizika PUBLIC include)

link_directories(/home/weiszben/vendor/glad-core-4.5)

target_link_libraries(Fizika glfw ${OPENGL_opengl_LIBRARY})
target_link_libraries(Fizika glfw ${OPENGL_glx_LIBRARY})