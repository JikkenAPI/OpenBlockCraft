//-----------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2017 Jeff Hutchinson
// Copyright (c) 2017 Tim Barnes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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