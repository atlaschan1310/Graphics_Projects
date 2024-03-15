FILE(GLOB_RECURSE imgui_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imgui/)

add_library(imgui ${imgui_sources})
target_link_libraries(imgui GLEW::GLEW)
target_link_libraries(imgui glfw)

