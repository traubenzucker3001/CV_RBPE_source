cmake_minimum_required(VERSION 2.8)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(
	${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
	${ASSIMP_INCLUDE_PATH}
	#${CVK_INCLUDE_PATH}
	${CUDA_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${CMAKE_SOURCE_DIR}/src/libraries/
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLFW_INCLUDE_GLCOREARB)
add_definitions(-DBUILD_SHARED_LIBS=off)

add_executable(${ProjectId} ${SOURCES} ${HEADER})

target_link_libraries(
    ${ProjectId}
    ${ALL_LIBRARIES}
	${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OpenGL3_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${CUDA_LIBRARIES}
)