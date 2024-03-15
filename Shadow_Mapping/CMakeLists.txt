cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "vcpkg toolchain file")

project(exercise)

set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shader")
add_compile_definitions(RES_DIR="${CMAKE_SOURCE_DIR}/res")

add_subdirectory(imgui)
include_directories(${CMAKE_SOURCE_DIR})

FILE(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw glm::glm imgui)

