if (NOT MSVC)
	# Force C++14 standard on compilers, MSVC already does this by default
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
	# Force multiple processors to be used to compile visual studio builds.
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()