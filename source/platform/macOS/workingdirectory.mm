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

#import <Cocoa/Cocoa.h>

namespace Platform
{
	void setWorkingDirectory(const char *dir)
	{
		NSString *path = [[NSBundle mainBundle] bundlePath];
		
		// For debugging, sometimes we want to put the executable within the bin folder.
		// We have to strip off the bin folder.
		if ([[path lastPathComponent] isEqualToString:@"bin"])
		{
			// strip off bin
			path = [path stringByDeletingLastPathComponent];
		}
		
		[[NSFileManager defaultManager] changeCurrentDirectoryPath:path];
	}
}