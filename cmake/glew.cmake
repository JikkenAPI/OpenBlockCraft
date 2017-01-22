add_library(GLEW STATIC thirdparty/glew/src/glew.c)
target_include_directories(GLEW PUBLIC thirdparty/glew/include)
target_compile_definitions(GLEW PUBLIC GLEW_STATIC)