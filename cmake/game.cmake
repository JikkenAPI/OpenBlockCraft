set (GAME_SOURCE
	source/core/singleton.hpp

	source/main.cpp
	
	source/platform/platform.hpp
	source/platform/timer.hpp
	source/platform/window.hpp
	source/platform/windowManager.cpp
	source/platform/windowManager.hpp

	source/platform/glfw/GLFWPlatform.cpp
	source/platform/glfw/GLFWTimer.cpp
	source/platform/glfw/GLFWTimer.hpp
	source/platform/glfw/GLFWWindow.cpp
	source/platform/glfw/GLFWWindow.hpp
	source/platform/glfw/GLFWWindowManager.cpp
	source/platform/glfw/GLFWWindowManager.hpp	
)

set (GAME_LIBS
	glfw
	OpenSimplexNoise
	GLEW
)

set (GAME_INCLUDES 
	source
	thirdparty/glew/include
	thirdparty/glfw/include
	thirdparty/glm
	thirdparty/openSimplexNoise
)

if (WIN32)
	set (GAME_LIBS
		${GAME_LIBS}
		OpenGL32
	)
endif()

add_executable(CPPCraft ${GAME_SOURCE})
target_link_libraries(CPPCraft ${GAME_LIBS})
target_include_directories(CPPCraft PUBLIC ${GAME_INCLUDES})
target_compile_definitions(CPPCraft PUBLIC GLEW_STATIC)

source_group("core" REGULAR_EXPRESSION core/*)
source_group("platform" REGULAR_EXPRESSION platform/*)
source_group("platform\\glfw" REGULAR_EXPRESSION platform/glfw/*)
source_group("main" main.cpp)