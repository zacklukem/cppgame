cmake_minimum_required(VERSION 3.13)
project(cppgame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cppgame 
  src/main.cc
  src/window.cc
  src/renderer.cc
  src/vbo.cc
  src/vao.cc
  src/shader.cc
  src/camera.cc
  src/input.cc
  src/chunk.cc
  src/chunkmesh.cc
  src/image_util.cc
)

target_include_directories(cppgame
  PRIVATE
  src/
)

# Add glad library
add_subdirectory(lib/glad/)

# Add glfw library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw/)

# Add glm library
add_subdirectory(lib/glm/)

# add noise library
add_subdirectory(lib/noise/)

target_link_libraries(cppgame
  PRIVATE glad
  PRIVATE glfw
  PRIVATE glm
  PRIVATE noise)


