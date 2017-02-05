cmake_minimum_required(VERSION 3.2)
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw")
include("cmake/compilerFlags.cmake")
include("cmake/openSimplexNoise.cmake")
include("cmake/glew.cmake")
include("cmake/game.cmake")

# Windows abi linkage
if (WIN32)
	include("cmake/msvcAbi.cmake")
endif()