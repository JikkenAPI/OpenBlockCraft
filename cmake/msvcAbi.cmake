#---BEGIN COPY FROM BULLET CMAKE---

# License: zlib
#
# Bullet Continuous Collision Detection and Physics Library
# http://bulletphysics.org
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

#We statically link to reduce dependancies
FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO )
	IF(${flag_var} MATCHES "/MD")
		STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	ENDIF(${flag_var} MATCHES "/MD")
	IF(${flag_var} MATCHES "/MDd")
		STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
	ENDIF(${flag_var} MATCHES "/MDd")
ENDFOREACH(flag_var)
#---END COPY FROM BULLET CMAKE---