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

#ifndef _CORE_FILE_HPP_
#define _CORE_FILE_HPP_

#include <string>
#include <cstdio>

class File
{
public:
	enum AccessFlags
	{
		eREAD = 1,
		eWRITE = 2,
		eBINARY = 4
	};

	File(const std::string &file, AccessFlags flag);
	~File();

	bool isFile() const;

	/// Gets the size of the file in bytes.
	/// @param [OUT] size The size in bytes, or -1 if the file does not exist.
	/// @return true if the file exists and it was able to grab the file size,
	///  false otherwise.
	bool getFileSize(long &size);

	bool readFile(std::string &contents);

	bool write(const std::string &contents);

private:
	std::FILE *mFilePtr;
	std::string mFile;
	AccessFlags mFlag;
};

#endif