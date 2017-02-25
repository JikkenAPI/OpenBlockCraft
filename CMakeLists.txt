cmake_minimum_required(VERSION 3.2)

if (WIN32)
	# Windows abi linkage
	# http://stackoverflow.com/a/20804336
	include("cmake/msvcAbi.cmake")
elseif(APPLE)
	# MacOS SDK. We are assuming the 10.11 SDK with XCode 7.X.X for now.
	# XCode 8 is unsupported currently.
	set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw")
include("cmake/compilerFlags.cmake")
include("cmake/openSimplexNoise.cmake")
include("cmake/game.cmake")

# We need JIKKEN, the cross platform rendering API from a separate repository
set(JIKKEN_PATH "" CACHE PATH "Path to the Jikken rendering API library")
if (JIKKEN_PATH STREQUAL "")
	message(SEND_ERROR "Please set the Jikken render path to the root folder of the Jikken library.")
else()
	# http://stackoverflow.com/a/15570165
	add_subdirectory(${JIKKEN_PATH} "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jikken")
endif()
