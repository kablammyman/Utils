// FileUtils.h
#pragma once

#include <vector>
#include <string>
#include <list>

using namespace std;

class FileUtils
{
public:
	
	//typedef DirNode*  FileNodeHandle;

	//these methods dont deal with dirTree, but will eventually.
	static  int GetNumFoldersinDir(string &path);

    static  vector<string> GetAllFolderNamesInDir(string &path);

	static string GetFileExt(string fullPath);

	static string GetFileNameNoExt(string &fullPath);

	static  int GetNumFilesInDir(string &path, string ext="");

    static  vector<string> GetAllFileNamesInDir(string &path,string ext ="",bool includePath = false);

	static  bool Delete_File(string file, bool permanetDelete = false);

	static  string DeleteAllFilesInDir(string &path);

	static  string GetRandomDirQuick(string &path);

	static  string GetRandomFileQuick(string &path);
		
	static  string GetFileNameFromPathString(string &path);
		
	static  string GetPathFromFullyQualifiedPathString(string &path);

	static  long long GetDirSize(string &path);

	static  bool DoesPathExist(string &path);

	static bool DoesFileExist(string &path);

	static  bool Move_File(string oldName, string newName);

	//the rest deals with dirTree
	static  void ClearDirTree();

	static  void AddDirTree(string &path, int numThreads = 0/*, DirNode* parent = 0*/);

	//static  FileNodeHandle getDirTreeRoot();
	//static  FileNodeHandle getDirTree(string &path);
	static  void StartDirTreeStep(string &path);
	//static  FileNodeHandle nextDirTreeStep();
	static  string NextDirTreeStep();
	static  bool IsFinished();
	static  size_t GetCurNodeNumFiles();
	static  size_t GetCurNodeNumFolders();
	static  list<string> GetCurNodeFileList();
	//static  string FileNodeHandleToString(FileNodeHandle dirNode);
	static  int GetNumFilesInTree(string &path);
	//static  int getNumFilesInTree(FileNodeHandle dirNode);
	//static  void ToString(FileNodeHandle dirNode);

	static  int GetNumDirsInTree(string &path);

	static  void DumpTreeToVector(string &path, vector<string> &ret);

	static  int Test();
		
};
