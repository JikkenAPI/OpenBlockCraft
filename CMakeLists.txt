cmake_minimum_required(VERSION 3.2)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw")
include("cmake/compilerFlags.cmake")
include("cmake/openSimplexNoise.cmake")
include("cmake/glew.cmake")
include("cmake/game.cmake")

# Windows abi linkage
if (WIN32)
	include("cmake/msvcAbi.cmake")
endif()