#ifndef INC_FILE_STRUCT_H
#define INC_FILE_STRUCT_H

#include <string>
#include <iostream>
#include <list>

using namespace std;

class DirNode
{
private:
	string path;
public:
	string GetFileName();
	string GetFileExt();
	string GetParentDir();
	list<string> fileList;//all the files
	list<DirNode *> childDir;
	DirNode *GetChildDir(string dir);
	string FormatPathForPrinting();
	string ToString();
	
	DirNode(string p)
	{
		fileList.clear();
		path = p;
	}

	~DirNode()
	{
		fileList.clear();
		//an external/managing class will clean up the actual data, 
		//we will clean up the pointer list only
		childDir.clear();
		path.clear();
	}

	bool IsPathName(string dir)
	{
		return dir == path;
	}
	size_t GetNumFiles()
	{
		return fileList.size();
	}
	size_t GetNumFolders()
	{
		return childDir.size();
	}
};
#endif //INC_FILE_STRUCT_H