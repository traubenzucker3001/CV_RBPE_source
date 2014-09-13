project(OpenGLProject)

include(${CMAKE_MODULE_PATH}/doxygen.cmake)
include(${CMAKE_MODULE_PATH}/macros.cmake)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

find_package(OpenGL3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(BULLET REQUIRED)
find_package(LIBOVR REQUIRED)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
GENERATE_SUBDIRS(ALL_LIBRARIES ${CMAKE_SOURCE_DIR}/src/libraries)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
GENERATE_SUBDIRS(ALL_EXECUTABLES ${CMAKE_SOURCE_DIR}/src/executables)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/shaders)