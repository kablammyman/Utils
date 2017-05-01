//last update 10-22-11
#ifndef INC_DIR_TREE_H
#define INC_DIR_TREE_H



#include <string>
#include <vector>
#include <list>
#include <stack>
#include "FileStruct.h"

using namespace std;
/*
we can derive the info we want when we want it
the new struct should prob look like this:
struct fileStruct
{
	string getName();
	string getExt();
	string path;
	string getParentDir();
	list<string> fileAndFolderList;//all the files and folders inside this dir
	//bool isDir;//this is always true now
}
*/

#ifdef _WIN32
#define SLASH  '\\'
#else
#define SLASH = '/'
#endif

class DirectoryTree
{
	

	int totalFilesInTree;
	int totalDirsInTree;
	DirNode * dirRoot;
	list<string> ignoreList;
	
	//points the the latest node we just created, we use this when we need to do things in stemps
	//we have this instead of making an opaque pointer. this technique is temporary until i can
	//figure out how to make my opaque pointer without including many other private header files
	DirNode *curNodeInStep;
	bool endOfProcessing;
	int curTreeLevel;
	//moved out of process fn()s so i can have the options to process 1 dir at at time
	//this way i can do work as the tree builds
	stack<DirNode *> dirStack;

	
	void ProcessSingleDir(DirNode *curNode, vector<string> &childrenDirs);

	public:
		
		DirectoryTree()
		{
			dirRoot = 0;
			curNodeInStep = 0;
			totalFilesInTree = 0;
			totalDirsInTree = 0;
		}
		~DirectoryTree()
		{
			ClearAll();
		}
		//should only really matter when we have an unfishied stack, but we need to clear it to start over
		//the other objects will be in the actual dir tree once they are processed and not in the stack
		void ClearStack()
		{
			while (!dirStack.empty())
			{
				DirNode *temp = dirStack.top();
				if(temp != 0)
					delete temp;
				dirStack.pop();
			}
		}
		
		void ProcessFilesFromDir(string path);
		void ProcessFilesFromDirMT(string path, int maxThreads =10);
		
		void StartProcessFilesFromDirStep(string path);
		DirNode*  NextProcessFilesFromDirStep();
		
		//this change so it works with sub trees too
		DirNode *GetDirTreeRoot() { return dirRoot; }
		int GetNumFilesInTree() {return totalFilesInTree;}
		int GetNumDirsInTree() { return totalDirsInTree; }
		//only valid when processing one step at a time
		bool IsFinihsedProcessing() { return endOfProcessing; }
		void DumpTreeToFile(string path);
		void DumpTreeToVector(vector<string> &returnVec);
		
		DirNode * GetDirNode(string path);
		DirNode * GetcurNodeInStep() { return curNodeInStep; }

		void AddToIgnoreList(string badFile);
		bool IsInIgnoreList(string badFile);
		string GetRandomFile();
		
		static long long GetDirSize(string path);

		static string GetFileNameFromPathString(string path);
		static string GetPathFromFullyQualifiedPathString(string path);
		static string PrepPathForTraversal(string path);
		void ClearAll();
};
#endif
