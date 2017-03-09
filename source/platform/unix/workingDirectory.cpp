//-----------------------------------------------------------------------------
// OpenBlockCraft -- A 3D block based game
// Copyright(C) 2017 Jeff Hutchinson
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>
//-----------------------------------------------------------------------------
#include <unistd.h>
#include <string>

namespace Platform
{
	void setWorkingDirectory(const char *dir)
	{
		std::string path(dir);
		path = path.substr(0, path.find_last_of("/"));
		if (chdir(path.c_str()) != 0 )
		{
			std::printf("Failed to set working directory to: %s", path.c_str());
		}
	}
}