//------------------------------------------------------------------------
//
//  Eureka DOOM Editor
//
//  Copyright (C) 2020 Ioan Chera
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

#include "lib_file.h"
#include "m_strings.h"
#include "testUtils/TempDirContext.hpp"
#include "gtest/gtest.h"
#include "FL/filename.H"
#include <fstream>

void DebugPrintf(const char *str, ...)
{
}

class LibFileTempDir : public TempDirContext
{
};

TEST(LibFile, HasExtension)
{
	ASSERT_FALSE(HasExtension("man/doc."));
	ASSERT_TRUE(HasExtension("man/doom.wad"));
	ASSERT_FALSE(HasExtension("man.wad/doom"));
	ASSERT_TRUE(HasExtension("man.wad/doom.wad"));
	ASSERT_TRUE(HasExtension("man.wad/doom..wad"));
	ASSERT_FALSE(HasExtension(".okay"));
	ASSERT_FALSE(HasExtension("man/.okay"));
	ASSERT_TRUE(HasExtension("man/..okay"));
	ASSERT_FALSE(HasExtension("man/.okay."));
	ASSERT_TRUE(HasExtension("man/.okay.wad"));
	ASSERT_FALSE(HasExtension("/."));
	ASSERT_FALSE(HasExtension("."));
	ASSERT_FALSE(HasExtension(".."));
	ASSERT_FALSE(HasExtension(""));
}

TEST(LibFile, MatchExtension)
{
	ASSERT_TRUE(MatchExtension("man/doc.", nullptr));
	ASSERT_TRUE(MatchExtension("man/doc.", ""));
	ASSERT_FALSE(MatchExtension("man/.doc.", "doc"));
	ASSERT_TRUE(MatchExtension("man/doc. ", " "));
	ASSERT_TRUE(MatchExtension("man.wad/doom", nullptr));
	ASSERT_FALSE(MatchExtension("man.wad/doom", "doom"));
	ASSERT_TRUE(MatchExtension("man.wad/doom.wad", ".WAD"));
	ASSERT_TRUE(MatchExtension("man.wad/doom..wad", ".WAD"));
	ASSERT_TRUE(MatchExtension(".okay", ""));
	ASSERT_FALSE(MatchExtension(".okay", "okay"));
	ASSERT_TRUE(MatchExtension("man/.okay", ""));
	ASSERT_FALSE(MatchExtension("man/.okay", "okay"));
	ASSERT_TRUE(MatchExtension("man/.okay.WAD", "wad"));
	ASSERT_TRUE(MatchExtension("/.", nullptr));
	ASSERT_TRUE(MatchExtension(".", nullptr));
	ASSERT_TRUE(MatchExtension("..", nullptr));
	ASSERT_FALSE(MatchExtension("..", "."));
	ASSERT_TRUE(MatchExtension("", nullptr));
}

TEST(LibFile, ReplaceExtension)
{
	ASSERT_EQ(ReplaceExtension("man/doc.", "wad"), "man/doc..wad");
	ASSERT_EQ(ReplaceExtension("man/doc.", "WAD"), "man/doc..WAD");
	ASSERT_EQ(ReplaceExtension("man/doc.", ""), "man/doc.");
	ASSERT_EQ(ReplaceExtension("man/doc.", nullptr), "man/doc.");
	ASSERT_EQ(ReplaceExtension("man/.doc", ""), "man/.doc");
	ASSERT_EQ(ReplaceExtension("man/.doc", nullptr), "man/.doc");
	ASSERT_EQ(ReplaceExtension("man/.doc", "wad"), "man/.doc.wad");
	ASSERT_EQ(ReplaceExtension("man.wad/doom", "waD"), "man.wad/doom.waD");
	ASSERT_EQ(ReplaceExtension("man.wad/doom.wad", ".txt"), "man.wad/doom.txt");
	ASSERT_EQ(ReplaceExtension("man.wad/doom.wad", ""), "man.wad/doom");
	ASSERT_EQ(ReplaceExtension("man.wad/doom.wad", nullptr), "man.wad/doom");
	ASSERT_EQ(ReplaceExtension("man.wad/doom..wad", nullptr), "man.wad/doom.");
	ASSERT_EQ(ReplaceExtension("man.wad/doom..wad", ""), "man.wad/doom.");
	ASSERT_EQ(ReplaceExtension("man.wad/doom..wad", "txt"), "man.wad/doom..txt");
	ASSERT_EQ(ReplaceExtension(".okay", "txt"), ".okay.txt");
	ASSERT_EQ(ReplaceExtension(".okay", ""), ".okay");
	ASSERT_EQ(ReplaceExtension(".okay", nullptr), ".okay");
	ASSERT_EQ(ReplaceExtension("/.", nullptr), "/.");
	ASSERT_EQ(ReplaceExtension("/.", "txt"), "/..txt");
	ASSERT_EQ(ReplaceExtension(".", "txt"), "..txt");
	ASSERT_EQ(ReplaceExtension("", "txt"), ".txt");
	ASSERT_EQ(ReplaceExtension(".", ""), ".");
	ASSERT_EQ(ReplaceExtension(".", nullptr), ".");
	ASSERT_EQ(ReplaceExtension("..", ""), "..");
	ASSERT_EQ(ReplaceExtension("..", nullptr), "..");
	ASSERT_EQ(ReplaceExtension("..", "txt"), "...txt");
	ASSERT_EQ(ReplaceExtension("..txt", "wad"), "..wad");
	ASSERT_EQ(ReplaceExtension("..txt", ""), ".");
	ASSERT_EQ(ReplaceExtension("..txt", nullptr), ".");
	ASSERT_EQ(ReplaceExtension("", ""), "");
	ASSERT_EQ(ReplaceExtension("", nullptr), "");
	ASSERT_EQ(ReplaceExtension("", "wad"), ".wad");
}

TEST(LibFile, FilenameGetPath)
{
    ASSERT_EQ(FilenameGetPath("path/to/file"), "path/to");
    ASSERT_EQ(FilenameGetPath("path/to" DIR_SEP_STR DIR_SEP_STR "file"), "path/to");
    ASSERT_EQ(FilenameGetPath("file"), ".");
	ASSERT_EQ(FilenameGetPath(""), ".");
    ASSERT_EQ(FilenameGetPath(DIR_SEP_STR "file"), DIR_SEP_STR);
	ASSERT_EQ(FilenameGetPath("/doom/"), "/doom");
	ASSERT_EQ(FilenameGetPath("///doom.wad"), "/");
#ifdef _WIN32
    ASSERT_EQ(FilenameGetPath("C:" DIR_SEP_STR "file"), "C:" DIR_SEP_STR);
#endif
}

TEST(LibFile, GetBaseName)
{
	ASSERT_EQ(GetBaseName("path/to///fileA.wad"), "fileA.wad");
    ASSERT_EQ(GetBaseName("path/to/fileB"), "fileB");
    ASSERT_EQ(GetBaseName("/fileC.txt"), "fileC.txt");
    ASSERT_EQ(GetBaseName("//file"), "file");
    ASSERT_EQ(GetBaseName("fil"), "fil");
	ASSERT_EQ(GetBaseName(""), "");
}

TEST(LibFile, FilenameIsBare)
{
	ASSERT_TRUE(FilenameIsBare(""));
	ASSERT_TRUE(FilenameIsBare("Doom"));
	ASSERT_TRUE(FilenameIsBare("DOOM"));
	ASSERT_TRUE(FilenameIsBare("doom"));
	ASSERT_FALSE(FilenameIsBare("/doom"));
	ASSERT_FALSE(FilenameIsBare(":doom"));
	ASSERT_FALSE(FilenameIsBare("/doom.wad"));
	ASSERT_FALSE(FilenameIsBare("doom.wad"));
	ASSERT_FALSE(FilenameIsBare("C:\\doom"));
	ASSERT_FALSE(FilenameIsBare("\\doom"));
}

TEST(LibFile, FilenameReposition)
{
	ASSERT_EQ(FilenameReposition("/doom/doom.wad", "doom2/"), "doom2/doom.wad");
	ASSERT_EQ(FilenameReposition("/wherever/doom.wad", "/"), "/doom.wad");
	ASSERT_EQ(FilenameReposition("doom.wad", "/"), "/doom.wad");
	ASSERT_EQ(FilenameReposition("/drag/doom.wad", nullptr), "doom.wad");
	ASSERT_EQ(FilenameReposition("doom.wad", "lamb/"), "lamb/doom.wad");
	ASSERT_EQ(FilenameReposition("", "lamb/"), "lamb/");
	ASSERT_EQ(FilenameReposition(nullptr, nullptr), "");
	ASSERT_EQ(FilenameReposition("/manipulate/doom.wad", "lamb"), "doom.wad");
	ASSERT_EQ(FilenameReposition("/manipulate/doom.wad", "/abc/def/lamb"), "/abc/def/doom.wad");
}

TEST_F(LibFileTempDir, GetAbsolutePath)
{
	char path[FL_PATH_MAX];
	int result = fl_filename_absolute(path, sizeof(path), mTempDir.c_str());
	ASSERT_NE(result, 0);
	SString stringResult = GetAbsolutePath(mTempDir);
	ASSERT_STREQ(stringResult.c_str(), path);
}

TEST_F(LibFileTempDir, FileExists)
{
	SString path = getChildPath("hello");
	ASSERT_FALSE(FileExists(path));
	std::ofstream os(path.get());
	ASSERT_TRUE(os.is_open());
	os.close();
	mDeleteList.push_back(path);
	ASSERT_TRUE(FileExists(path));

	int result = remove(path.c_str());
	ASSERT_EQ(result, 0);
	mDeleteList.clear();

	// Deleted, now must be back to false
	ASSERT_FALSE(FileExists(path));
}

TEST_F(LibFileTempDir, FileCopy)
{
	SString path = getChildPath("file1");

	std::ofstream os(path.get());
	ASSERT_TRUE(os.is_open());
	mDeleteList.push_back(path);
	os << "HelloWorld!";
	os.close();

	SString path2 = getChildPath("file2");
	bool result = FileCopy(path, path2);
	ASSERT_TRUE(result);
	mDeleteList.push_back(path2);

	std::ifstream is(path2.get());
	ASSERT_TRUE(is.is_open());
	std::string str;
	is >> str;
	ASSERT_TRUE(is.eof());
	is.close();

	ASSERT_EQ(str, "HelloWorld!");

	// Now attempt other impossible operations
	// Inexisting source
	result = FileCopy(getChildPath("file3"), path2);
	ASSERT_FALSE(result);
	// Bad kind of destination
	result = FileCopy(path, mTempDir);
	ASSERT_FALSE(result);
	// Inexistent folder
	result = FileCopy(path, getChildPath("subpath/file"));
	ASSERT_FALSE(result);

	// Now test overwriting
	std::ofstream os2(path.get(), std::ios::trunc);
	ASSERT_TRUE(os2.is_open());
	os2 << "SecondSight";
	os2.close();

	result = FileCopy(path, path2);
	ASSERT_TRUE(result);

	std::ifstream is2(path2.get());
	ASSERT_TRUE(is2.is_open());
	str.clear();
	is2 >> str;
	ASSERT_TRUE(is2.eof());
	ASSERT_EQ(str, "SecondSight");
	is2.close();
}
