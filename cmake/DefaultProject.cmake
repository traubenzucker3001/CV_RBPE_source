project(OpenGLProject)

include(${CMAKE_MODULE_PATH}/doxygen.cmake)
include(${CMAKE_MODULE_PATH}/macros.cmake)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

find_package(GLM REQUIRED)


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
GENERATE_SUBDIRS(ALL_LIBRARIES ${CMAKE_SOURCE_DIR}/src/libraries)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
GENERATE_SUBDIRS(ALL_EXECUTABLES ${CMAKE_SOURCE_DIR}/src/executables)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/shaders)

#---include cvk---
include(ExternalProject)
ExternalProject_Add(
	cvk
	SOURCE_DIR ${DEPENDENCIES_PATH}/cvk
	INSTALL_COMMAND "" )
ExternalProject_Get_Property(cvk install_dir)
add_library(cvkLib STATIC IMPORTED)
set_property(TARGET cvkLib PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libCVK_Framework.a)
add_dependencies(cvkLib cvk)