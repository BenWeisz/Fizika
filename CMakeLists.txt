cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Fizika VERSION 1.0)

add_definitions(-std=c++17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(VENDOR_DIR "~/vendor")
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

add_executable(Fizika main.cpp)

find_package(glfw3 3.3 REQUIRED)
# find_package(OpenGL REQUIRED)

# target_include_directories(Fizika PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(Fizika PUBLIC include)
target_include_directories(Fizika PUBLIC ${VENDOR_DIR}/glad-core-4.5/include)

target_link_libraries(Fizika glfw ${OPENGL_opengl_LIBRARY})
target_link_libraries(Fizika glfw ${OPENGL_glx_LIBRARY})

target_sources(Fizika PUBLIC
    src/Shader.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/VertexBufferLayout.cpp
    ${VENDOR_DIR}/glad-core-4.5/src/glad.c
)

if(${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif()