//last update 10-22-11
#include "directory.h"
#include "FileStruct.h"


//since some projects that i build for doesnt support c++11 stuff, i need to makre sure i disable it when building for them
#if __cplusplus > 199711L
#include <thread>
#include <mutex>
//#include <future>         // std::async, std::future
#endif

void directoryTree::addToIgnoreList(string badFile)
{
	list<string>::iterator it;
	bool inList = false;
	
	for ( it=ignoreList.begin() ; it != ignoreList.end(); it++ )
		if(*it == badFile)
			inList = true;

	if(!inList)
		ignoreList.push_back(badFile);
}

bool directoryTree::isInIgnoreList(string badFile)
{
	list<string>::iterator it;
	
	for ( it=ignoreList.begin() ; it != ignoreList.end(); it++ )
		if(*it == badFile)
			return true;

	return false;
}

void directoryTree::processSingleDir(DirNode *curNode, vector<string> &childrenDirs)
{
	WIN32_FIND_DATA FindFileData;

	string curDir = PrepPathForTraversal(curNode->ToString());

	HANDLE hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk
	
	do {
		if (isInIgnoreList(FindFileData.cFileName))//ignore anything we put in this list
			continue;

		//if we find a directory, add its name to the stack, so we can parse it later
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			string newPath = curDir + FindFileData.cFileName + "\\";
			childrenDirs.push_back(newPath);
			totalDirsInTree++;
		}
		//else this is a file, so add it to the list of files for this dir
		else
		{
			totalFilesInTree++;
			curNode->fileList.push_back(FindFileData.cFileName);
		}

	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);
	
}
void directoryTree::processFilesFromDir(string path)//needs to have *.fileExt to work
{
	clearStack();

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	totalFilesInTree = 0;
	addToIgnoreList(".");
	addToIgnoreList("..");
	addToIgnoreList("Thumbs.db");

	//if we want to clear the data firs, we must explicitly call it now

	do {
		DirNode *curNode = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack

		vector<string> childDirs;
		processSingleDir(curNode, childDirs);

		for (size_t i = 0; i < childDirs.size(); i++)
		{
			//create a new child node now, so we can process it later
			DirNode *childNode = new DirNode(childDirs[i]);

			curNode->childDir.emplace_back(childNode);
			dirStack.push(childNode);
		}

	} while (!dirStack.empty());
}
void directoryTree::startProcessFilesFromDirStep(string path)//needs to have *.fileExt to work
{
	clearStack();

	addToIgnoreList(".");
	addToIgnoreList("..");
	addToIgnoreList("Thumbs.db");

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	endOfProcessing = false;
	totalFilesInTree = 0;
	curNodeInStep = NULL;
}

DirNode* directoryTree::nextProcessFilesFromDirStep()//needs to have *.fileExt to work
{
	DirNode *curNode = dirStack.top(); //get the next dr to process
	dirStack.pop(); //take it off of stack

	vector<string> childDirs;
	processSingleDir(curNode, childDirs);

	for (size_t i = 0; i < childDirs.size(); i++)
	{
		//create a new child node now, so we can process it later
		DirNode *childNode = new DirNode(childDirs[i]);

		curNode->childDir.emplace_back(childNode);
		dirStack.push(childNode);
	}

	if (dirStack.empty())
		endOfProcessing = true;

	curNodeInStep = curNode;
	return curNode;
}

void directoryTree::processFilesFromDirMT(string path, int maxThreads)
{
//since some projects that i build for doesnt support c++11 stuff, i need to makre sure i disable it when building for them
#if __cplusplus <= 199711L
	processFilesFromDir(path);
#elif

	
	int numThreads = 0;
	mutex stackLock;

	clearStack();

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	totalFilesInTree = 0;
	addToIgnoreList(".");
	addToIgnoreList("..");
	addToIgnoreList("Thumbs.db");

	//if we want to clear the data firs, we must explicitly call it now
	
	vector<vector<string>> childDirs(maxThreads, vector<string>(0));
	vector<DirNode *> curNode(maxThreads,NULL);
	
	//okay, now we need to make all of our threads
	
	//packaged_task<void(DirNode *, vector<string>&)> task(&directoryTree::processSingleDir);
	//future<void> *fut = new future<void>[maxThreads];
	//thread *t = new thread[maxThreads];

	do {

		//okay, now we need to make all of our threads
		thread *t = new thread[maxThreads];
		
		for (int i = 0; i < maxThreads; i++)
		{
			//if this object isnt in use, use it, other wise, look for one that isnt in use
			/*auto status = fut[i].wait_for(std::chrono::seconds(0));
			if(status == std::future_status::timeout)
				continue;*/

			stackLock.lock();

			if (dirStack.empty())
			{
				//unlock the mutex, so we dont crash trying ot lock it again!
				stackLock.unlock();
				break;
			}
			numThreads++;

			curNode[i] = dirStack.top(); //get the next dr to process
			dirStack.pop(); //take it off of stack

			stackLock.unlock();
			t[i] = thread(&directoryTree::processSingleDir, this, curNode[i], std::ref(childDirs[i]));

		}
		int curThreads = numThreads;
		for (int i = 0; i < curThreads; i++)
		{
			/*auto status = fut[i].wait_for(std::chrono::seconds(0));
			if (status == std::future_status::timeout)
				continue;*/
			
			//else if (status == std::future_status::ready)
			//{
			if (t[i].joinable())
				t[i].join();
			else
				continue;

				numThreads--;

				for (size_t j = 0; j < childDirs[i].size(); j++)
				{
					//create a new child node now, so we can process it later
					DirNode *childNode = new DirNode(childDirs[i][j]);

					curNode[i]->childDir.emplace_back(childNode);

					stackLock.lock();
					dirStack.push(childNode);
					stackLock.unlock();
				}
				childDirs[i].clear();
				//task.reset();
			//}
			/*else {
				// There is still std::future_status::defered
			}*/
		}
		delete[] t;
	} while (!dirStack.empty() );
#endif
}

vector<string> directoryTree::tokenize(string path, string delims)
{
	vector<string> returnVec;
	char *p = strtok(const_cast<char *>(path.c_str()), delims.c_str());
	while (p)
	{
		//printf ("Token: %s\n", p);
		returnVec.push_back(p);
		p = strtok(NULL, delims.c_str());
	}
	return returnVec;
}

DirNode * directoryTree::getDirNode(string path)
{
	if (*path.rbegin() != '\\')
		path += '\\';

	string curPath = dirRoot->ToString();
	size_t find = path.find(curPath);
	
	//make sure the path we are lookging for is within our tree
	if (find == string::npos)
		return NULL;

	string shortPath = path.substr(curPath.size());
	vector<string> tokens = tokenize(shortPath, "\\");

	size_t i = 0;
	
	DirNode *runner = dirRoot;
	
	//make sure the path we want matches the tree root 
	if (!runner->IsPathName(curPath))
		return NULL;
	//if so, then start the actual search
	curPath += ("\\" + tokens[i]+"\\");

	while (runner != NULL)
	{
		runner = runner->GetChildDir(curPath);
		if (runner == NULL)
			return NULL;

		if (i < tokens.size() - 1)
			i++;
		else
		{
			if (path == runner->ToString())
				return runner;
			return NULL;
		}
		curPath += (tokens[i]+"\\");
	}
	return NULL;
}
void directoryTree::dumpTreeToFile(string path)
{
	FILE *pFile;
	string fileName = path;

	fileName += ".txt";
	
	pFile = fopen (fileName.c_str(),"w");

	if (pFile==NULL)
		return;
	vector<string> pathList;

	dumpTreeToVector(pathList);

	for(size_t i = 0; i < pathList.size(); i++)
		fprintf(pFile,"%s\n", pathList[i].c_str() );
	
	fclose (pFile);
}

void directoryTree::dumpTreeToVector(vector<string> &returnVec)
{
	stack<DirNode *> dirStack;

	dirStack.push(dirRoot);
	while(!dirStack.empty())
	{
		DirNode *curNode = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack
		
		if (curNode == NULL)
			continue;
		returnVec.push_back(curNode->ToString());
		if (curNode->childDir.empty())
			continue;
		list<DirNode *>::iterator it;
		for (it = curNode->childDir.begin(); it != curNode->childDir.end(); it++)
			dirStack.push((*it));
	} 
}


string directoryTree::getRandomFile()
{
	/*int index = rand()%dirList.size();
	size_t randFile = dirList[index]->fileAndFolderList.size();
	
	list<string>::iterator it = dirList[index]->fileAndFolderList.begin(); 
	
	if (dirList[index]->fileAndFolderList.size() > randFile)
	{
		std::advance(it, randFile);// 'it' points to the element at index 'N'
	}
	return *it;*/

	return "";
}


//moved out of here, but i will need to use some of this code to make it work for the tree, so i left a copy
__int64 directoryTree::getDirSize(string path)//needs to have *.fileExt to work
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	
	__int64 size = 0;

	stack<string> dirStack;
	dirStack.push(path);

	do{
		string curDir = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack
		
		curDir +="*";//add asterisk to look for all "files" in cur dir
		hFind = FindFirstFile(curDir.c_str(), &FindFileData);

		size_t removeAst = curDir.length()-1;
		curDir.resize (removeAst); //curDir[removeAst]='\0';//remove the asterisk

	

		if(hFind != INVALID_HANDLE_VALUE)
		do{
 
		//if we find a directory, add its name to the stack, so we can parse it later
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				string newPath = curDir + FindFileData.cFileName + "\\";
				dirStack.push(newPath);
			}
			//else this is a file, so add it to the list of files for this dir
			else
			{
				LARGE_INTEGER sz;
                // All we want here is the file size.  Since file sizes can be larger
                // than 2 gig, the size is reported as two DWORD objects.  Below we
                // combine them to make one 64-bit integer.
                sz.LowPart = FindFileData.nFileSizeLow;
                sz.HighPart = FindFileData.nFileSizeHigh;
                size += sz.QuadPart;
			}

		}while(FindNextFile(hFind, &FindFileData));
		
	}while(!dirStack.empty());

	FindClose(hFind);
	return size;
}

string directoryTree::PrepPathForTraversal(string path)
{
	char lastChar = *path.rbegin();
	string newPath = path;
	//if the path is already prepped, then do nothing
	if(lastChar == '*')
		return path;

	else if(lastChar == '\\')
		newPath += "*";//add asterisk to look for all "files" in cur dir
	else
		newPath += "\\*";//if we forgot to add the last back slash, add it, so we dont crash
	return  newPath;
}
string directoryTree::getFileNameFromPathString(string path)
{
	size_t found = path.find_last_of("\\");
	if (found != string::npos)
		return path.substr(found + 1);
	return "";
}

string directoryTree::getPathFromFullyQualifiedPathString(string path)
{
	size_t found = path.find_last_of("\\");
	if (found != string::npos)
		return path.substr(0, found);
	return "";
}

void directoryTree::clearAll()
{
	ignoreList.clear();
	totalFilesInTree = 0;
	totalDirsInTree = 0;

	if (dirRoot == NULL)
		return;
	
	stack<DirNode *> dirStack;
	dirStack.push(dirRoot);
	
	while (!dirStack.empty())
	{
		DirNode *curNode = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack

		if (curNode == NULL)
			continue;

		if (!curNode->childDir.empty())
		{
			list<DirNode *>::iterator it;
			for (it = curNode->childDir.begin(); it != curNode->childDir.end(); it++)
				dirStack.push((*it));
		}

		delete curNode;
		curNode = NULL;
	}

	//now that everything is deleted, make sure we point the root to NULL
	dirRoot = NULL;
}