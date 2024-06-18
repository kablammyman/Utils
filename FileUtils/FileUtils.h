// FileUtils.h
#pragma once

#include <vector>
#include <string>
#include <list>
#include "DocumentTags.h"

using namespace std;

class FileUtils
{
public:
	
	//typedef DirNode*  FileNodeHandle;

	//these methods dont deal with dirTree, but will eventually.
	static  int GetNumFoldersinDir(string path);

    static  vector<string> GetAllFolderNamesInDir(string path);

	static string GetFileExt(string fullPath);

	static string GetFileNameNoExt(string fullPath);

	static  int GetNumFilesInDir(string path, string ext="");

    static  vector<string> GetAllFileNamesInDir(string path,string ext,bool includePath = false);
	//for some reason, the GetAllFileNamesInDir was cashing an issue with how the exe was being built so the method would crash when calling it (before it could execute)
	//so i made a copy of the fn() thats windows only to bug test...and it fixes the issue?? my code still used the original code and not the copy too
    static  vector<string> GetAllFileNamesInDir(string path);

	static  bool Delete_File(string file, bool permanetDelete = false);

	static  string DeleteAllFilesInDir(string path);

	static  string GetRandomDirQuick(string path);

	static  string GetRandomFileQuick(string path);
		
	static  string GetFileNameFromPathString(string path);
		
	static  string GetPathFromFullyQualifiedPathString(string path);

	static  long long GetDirSize(string path);

	static  bool DoesPathExist(string path);

	static bool DoesFileExist(string path);

	static  bool Move_File(string oldName, string newName);

	//the rest deals with dirTree
	static  void ClearDirTree();

	static  void AddDirTree(string path, int numThreads = 0/*, DirNode* parent = 0*/);

	//static  FileNodeHandle getDirTreeRoot();
	//static  FileNodeHandle getDirTree(string &path);
	static  void StartDirTreeStep(string path);
	//static  FileNodeHandle nextDirTreeStep();
	static  string NextDirTreeStep();
	static  bool IsFinished();
	static  size_t GetCurNodeNumFiles();
	static  size_t GetCurNodeNumFolders();
	static  list<string> GetCurNodeFileList();
	//static  string FileNodeHandleToString(FileNodeHandle dirNode);
	static  int GetNumFilesInTree();
	//static  int getNumFilesInTree(FileNodeHandle dirNode);
	//static  void ToString(FileNodeHandle dirNode);

	static  int GetNumDirsInTree();

	static  void DumpTreeToVector(vector<string> &ret);

	static int GetRandomInt(int min, int max);

	static bool CreateNewDir(string path);
	static  int Test();

	static bool CreateNewFileFromTagTemaplateFile(std::string templatePath, std::string newFilePath, DocumentTags tagMap);
		
};
