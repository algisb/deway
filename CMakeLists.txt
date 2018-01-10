cmake_minimum_required (VERSION 3.1.0)

project (deway)
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "./bin")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "./bin")
add_definitions(-D_GLIBCXX_CONCEPT_CHECKS)#debug flags
add_definitions(-D_GLIBCXX_DEBUG)
add_definitions(-D_GLIBCXX_DEBUG_PEDANTIC)

#glew------------------------------------------------------
#include_directories ("./external/glew/include")
add_subdirectory("./external/glew/build/cmake")
include_directories ("./external/glew/include")
add_definitions(-DGLEW_STATIC)



#opengl----------------------------------------------------
find_package(OpenGL)
include_directories( ${OPENGL_INCLUDE_DIRS} )


#glfw------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

include_directories ("./external/glfw/include")
add_subdirectory("./external/glfw")
add_definitions(-DGLFW_STATIC)

#tinyobjloader---------------------------------------------
include_directories ("./external/tinyobjloader")
#lodepng---------------------------------------------------
include_directories ("./external/lodepng")


#kelp engine-----------------------------------------------
include_directories ("./src")
file(GLOB KELP_SRC "./src/*.cpp" "./src/component/*.cpp" "./src/world/*.cpp" "./external/lodepng/*.cpp" )
file(GLOB KEP_SRC "./src/kep/*.cpp" "./src/kep/collisionDetection/*.cpp" "./src/kep/collisionDetection/broadphase/*.cpp" "./src/kep/collisionDetection/finephase/*.cpp")
add_executable(deway-demo ${KELP_SRC} ${KEP_SRC} )
target_link_libraries (deway-demo ${OPENGL_LIBRARIES} glfw glew_s )
