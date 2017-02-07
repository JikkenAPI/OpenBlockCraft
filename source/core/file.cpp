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

#include "core/file.hpp"

inline std::string convertAccessFlagsToCFlag(File::AccessFlags flag)
{
	std::string ret = "";
	if (flag & File::AccessFlags::eREAD)
		ret += "r";
	if (flag & File::AccessFlags::eWRITE)
		ret += "w";
	if (flag & File::AccessFlags::eBINARY)
		ret += "b";
	return ret;
}

File::File(const std::string &file, AccessFlags flag)
{
	mFile = file;
	mFlag = flag;

	mFilePtr = fopen(file.c_str(), convertAccessFlagsToCFlag(flag).c_str());
}

File::~File()
{
	if (mFilePtr != nullptr)
		fclose(mFilePtr);
}

bool File::isFile() const
{
	return mFilePtr != nullptr;
}

bool File::getFileSize(long &size)
{
	if (!isFile())
	{
		size = -1;
		return false;
	}

	fseek(mFilePtr, 0, SEEK_END);
	size = ftell(mFilePtr);
	rewind(mFilePtr);
	return true;
}

bool File::readFile(std::string &contents)
{
	// getFileSize checks if the file exists.
	long size;
	if (!getFileSize(size))
	{
		contents = "";
		return false;
	}

	// If we do not have read access open, fail.
	if (!(mFlag & File::AccessFlags::eREAD))
	{
		std::printf("Unable to read file %s. Missing read access flag!\n", mFile.c_str());
		return false;
	}


	char *buffer = new char[size + 1];
	memset(buffer, 0, sizeof(char) * (size + 1));
	fread(buffer, sizeof(char), static_cast<size_t>(size), mFilePtr);
	contents = buffer;
	delete[] buffer;

	return true;
}

bool File::write(const std::string &contents)
{
	if (!isFile())
		return false;

	// If we do not have write access open, fail.
	if (!(mFlag & File::AccessFlags::eWRITE))
	{
		std::printf("Unable to write file %s. Missing write access flag!\n", mFile.c_str());
		return false;
	}
	
	fwrite(contents.c_str(), sizeof(char), contents.length(), mFilePtr);
	return true;
}