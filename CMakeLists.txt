cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0076 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.6)


project(SphereCarving)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(glfw)
include(glm)
include(glew)

add_subdirectory(src)