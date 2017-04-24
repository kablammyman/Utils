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



class DirectoryTree
{
	string PrepPathForTraversal(string path);
	void processSingleDir(DirNode *curNode, vector<string> &childrenDirs);
	vector<string> tokenize(string path, string delims);
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
	public:
		static char SLASH;
		DirectoryTree()
		{
			dirRoot = NULL;
			curNodeInStep = NULL;
			totalFilesInTree = 0;
			totalDirsInTree = 0;
#ifdef _WIN32
SLASH = '\\';
#else
SLASH = '/';
#endif
		}
		~DirectoryTree()
		{
			clearAll();
		}
		//should only really matter when we have an unfishied stack, but we need to clear it to start over
		//the other objects will be in the actual dir tree once they are processed and not in the stack
		void clearStack()
		{
			while (!dirStack.empty())
			{
				DirNode *temp = dirStack.top();
				if(temp != NULL)
					delete temp;
				dirStack.pop();
			}
		}
		
		void processFilesFromDir(string path);
		void processFilesFromDirMT(string path, int maxThreads =10);
		
		void startProcessFilesFromDirStep(string path);
		DirNode*  nextProcessFilesFromDirStep();
		
		//this change so it works with sub trees too
		DirNode *getDirTreeRoot() { return dirRoot; }
		int getNumFilesInTree() {return totalFilesInTree;}
		int getNumDirsInTree() { return totalDirsInTree; }
		//only valid when processing one step at a time
		bool isFinihsedProcessing() { return endOfProcessing; }
		void dumpTreeToFile(string path);
		void dumpTreeToVector(vector<string> &returnVec);
		
		DirNode * getDirNode(string path);
		DirNode * getcurNodeInStep() { return curNodeInStep; }

		void addToIgnoreList(string badFile);
		bool isInIgnoreList(string badFile);
		string getRandomFile();
		
		__int64 getDirSize(string path);

		static string DirectoryTree::getFileNameFromPathString(string path);
		static string DirectoryTree::getPathFromFullyQualifiedPathString(string path);

		void clearAll();
};
#endif
