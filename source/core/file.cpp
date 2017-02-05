//-----------------------------------------------------------------------------
// file.cpp
//
// Copyright(c) 2017 Jeff Hutchinson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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