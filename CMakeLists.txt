
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Fizika VERSION 1.0)

add_definitions(-std=c++17)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
set(VENDOR_DIR "~/vendor")
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

add_executable(Fizika main.cpp)

find_package(glfw3 3.3 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_include_directories(Fizika PUBLIC include)
target_include_directories(Fizika PUBLIC ${VENDOR_DIR}/glad-core-4.5/include)
target_include_directories(Fizika PUBLIC ${VENDOR_DIR}/tinyxml/include)
target_include_directories(Fizika PUBLIC ${VENDOR_DIR}/glm)

target_link_libraries(Fizika glfw ${OPENGL_opengl_LIBRARY})
target_link_libraries(Fizika glfw ${OPENGL_glx_LIBRARY})
target_link_libraries(Fizika Eigen3::Eigen)

target_sources(Fizika PUBLIC
    src/graphics/Shader.cpp
    src/graphics/VertexBuffer.cpp
    src/graphics/VertexArray.cpp
    src/graphics/VertexBufferLayout.cpp
    src/graphics/IndexBuffer.cpp
    src/graphics/Material.cpp
    src/graphics/Mesh.cpp
    src/graphics/Model.cpp
    src/input/Input.cpp
    ${VENDOR_DIR}/glad-core-4.5/src/glad.c
    ${VENDOR_DIR}/tinyxml/src/tinyxml2.cpp
)

if(${IS_DEBUG_BUILD})
    add_compile_definitions(__DEBUG__)
endif()