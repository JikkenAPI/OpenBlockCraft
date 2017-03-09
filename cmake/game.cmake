set (GAME_SOURCE
	source/core/geometry/cube.hpp
	source/core/file.cpp
	source/core/file.hpp
	source/core/frustrum.cpp
	source/core/frustrum.hpp
	source/core/noise.cpp
	source/core/noise.hpp
	source/core/singleton.hpp

	source/main.cpp
	
	source/platform/platform.hpp
	source/platform/timer.hpp
	source/platform/window.hpp
	source/platform/windowManager.cpp
	source/platform/windowManager.hpp

	source/platform/glfw/GLFWInputKeys.cpp
	source/platform/glfw/GLFWPlatform.cpp
	source/platform/glfw/GLFWTimer.cpp
	source/platform/glfw/GLFWTimer.hpp
	source/platform/glfw/GLFWWindow.cpp
	source/platform/glfw/GLFWWindow.hpp
	source/platform/glfw/GLFWWindowManager.cpp
	source/platform/glfw/GLFWWindowManager.hpp

	source/platform/input/iInputListener.hpp
	source/platform/input/inputEvents.hpp
	source/platform/input/inputKeys.hpp
	source/platform/input/inputManager.cpp
	source/platform/input/inputManager.hpp

	source/scene/camera.cpp
	source/scene/camera.hpp
	source/scene/sceneObject.cpp
	source/scene/sceneObject.hpp

	source/scene/world/block.hpp
	source/scene/world/chunk.cpp
	source/scene/world/chunk.hpp
	source/scene/world/chunkManager.cpp
	source/scene/world/chunkManager.hpp
)

if (APPLE)
	set (GAME_SOURCE
		${GAME_SOURCE}
		
		source/platform/macOS/workingdirectory.mm
	)
elseif(WIN32)
	set (GAME_SOURCE
		${GAME_SOURCE}
		
		source/platform/win32/workingDirectory.cpp
	)
elseif(UNIX)
	set (GAME_SOURCE
		${GAME_SOURCE}
		
		source/platform/unix/workingDirectory.cpp
	)
endif()

set (GAME_LIBS
	glfw
	OpenSimplexNoise
	Jikken
)

set (GAME_INCLUDES 
	source
	thirdparty/glfw/include
	thirdparty/glm
	thirdparty/openSimplexNoise
)

#set game root directory so other cmake modules can find any includes/libs if needed
set(GAME_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
	set (GAME_LIBS
		${GAME_LIBS}
		OpenGL32
	)
elseif (APPLE)
	set (GAME_LIBS
		${GAME_LIBS}
		"-framework AppKit"
		"-framework Foundation"
		"-framework OpenGL"
		"-framework Cocoa"
	)
endif ()

add_executable(CPPCraft ${GAME_SOURCE})
target_link_libraries(CPPCraft ${GAME_LIBS})
target_include_directories(CPPCraft PUBLIC ${GAME_INCLUDES})
target_compile_definitions(CPPCraft PUBLIC GLEW_STATIC)

if(WIN32)
target_compile_definitions(CPPCraft PUBLIC GLFW_EXPOSE_NATIVE_WIN32)
elseif(APPLE)
target_compile_definitions(CPPCraft PUBLIC GLFW_EXPOSE_NATIVE_COCOA)
else()
target_compile_definitions(CPPCraft PUBLIC GLFW_EXPOSE_NATIVE_X11)
endif()

#output directory
set_target_properties(CPPCraft PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(CPPCraft PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set_target_properties(CPPCraft PROPERTIES DEBUG_POSTFIX "_debug")

source_group("core" REGULAR_EXPRESSION core/*)
source_group("core\\geometry" REGULAR_EXPRESSION core/geometry/*)
source_group("main" main.cpp)
source_group("platform" REGULAR_EXPRESSION platform/*)
source_group("platform\\glfw" REGULAR_EXPRESSION platform/glfw/*)
source_group("platform\\input" REGULAR_EXPRESSION platform/input/*)
source_group("scene" REGULAR_EXPRESSION scene/*)
source_group("scene\\world" REGULAR_EXPRESSION scene/world/*)

if (APPLE)
	source_group("platform\\MacOS" REGULAR_EXPRESSION platform/macOS/*)
elseif(WIN32)
	source_group("platform\\Win32" REGULAR_EXPRESSION platform/win32/*)
elseif(UNIX)
	source_group("platform\\Unix" REGULAR_EXPRESSION platform/unix/*)
endif()

# Set Visual Studio startup project(cmake 3.6+)
if((${CMAKE_VERSION} VERSION_EQUAL 3.6.0) OR (${CMAKE_VERSION} VERSION_GREATER 3.6.0) AND MSVC)
 set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT CPPCraft)
endif()