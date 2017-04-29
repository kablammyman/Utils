//last update 10-22-11
#include "StringUtils.h"
#include "directory.h"
#include "FileStruct.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

//since some projects that i build for doesnt support c++11 stuff, i need to makre sure i disable it when building for them
#if __cplusplus > 199711L
#include <thread>
#include <mutex>
//#include <future>         // std::async, std::future
#endif

void DirectoryTree::AddToIgnoreList(string badFile)
{
	list<string>::iterator it;
	bool inList = false;
	
	for ( it=ignoreList.begin() ; it != ignoreList.end(); it++ )
		if(*it == badFile)
			inList = true;

	if(!inList)
		ignoreList.push_back(badFile);
}

bool DirectoryTree::IsInIgnoreList(string badFile)
{
	list<string>::iterator it;
	
	for ( it=ignoreList.begin() ; it != ignoreList.end(); it++ )
		if(*it == badFile)
			return true;

	return false;
}

void DirectoryTree::ProcessSingleDir(DirNode *curNode, vector<string> &childrenDirs)
{
	

	string curDir = PrepPathForTraversal(curNode->ToString());

#ifdef _WIN32
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk
	
	do {
		if (IsInIgnoreList(FindFileData.cFileName))//ignore anything we put in this list
			continue;

		//if we find a directory, add its name to the stack, so we can parse it later
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			string newPath = curDir + FindFileData.cFileName + SLASH;
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
#else
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(curDir.c_str())) != 0) 
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != 0) 
		{
			if (IsInIgnoreList(ent->d_name))//ignore anything we put in this list
				continue;

			//if we find a directory, add its name to the stack, so we can parse it later
			if (ent->d_type == DT_DIR)
			{
				string newPath = curDir + ent->d_name + SLASH;
				childrenDirs.push_back(newPath);
				totalDirsInTree++;
			}
			//else this is a file, so add it to the list of files for this dir
			else if (ent->d_type == DT_REG)
			{
				totalFilesInTree++;
				curNode->fileList.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
		//return EXIT_FAILURE;
	}
#endif	
}
void DirectoryTree::ProcessFilesFromDir(string path)//needs to have *.fileExt to work
{
	ClearStack();

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	totalFilesInTree = 0;
	AddToIgnoreList(".");
	AddToIgnoreList("..");
	AddToIgnoreList("Thumbs.db");

	//if we want to clear the data firs, we must explicitly call it now

	do {
		DirNode *curNode = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack

		vector<string> childDirs;
		ProcessSingleDir(curNode, childDirs);

		for (size_t i = 0; i < childDirs.size(); i++)
		{
			//create a new child node now, so we can process it later
			DirNode *childNode = new DirNode(childDirs[i]);

			curNode->childDir.emplace_back(childNode);
			dirStack.push(childNode);
		}

	} while (!dirStack.empty());
}
void DirectoryTree::StartProcessFilesFromDirStep(string path)//needs to have *.fileExt to work
{
	ClearStack();

	AddToIgnoreList(".");
	AddToIgnoreList("..");
	AddToIgnoreList("Thumbs.db");

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	endOfProcessing = false;
	totalFilesInTree = 0;
	curNodeInStep = 0;
}

DirNode* DirectoryTree::NextProcessFilesFromDirStep()//needs to have *.fileExt to work
{
	DirNode *curNode = dirStack.top(); //get the next dr to process
	dirStack.pop(); //take it off of stack

	vector<string> childDirs;
	ProcessSingleDir(curNode, childDirs);

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

void DirectoryTree::ProcessFilesFromDirMT(string path, int maxThreads)
{
//since some projects that i build for doesnt support c++11 stuff, i need to makre sure i disable it when building for them
#if _WIN32	
#if __cplusplus <= 199711L
	maxThreads = 1;
	ProcessFilesFromDir(path);
#elif

	
	int numThreads = 0;
	mutex stackLock;

	clearStack();

	dirRoot = new DirNode(path);
	dirStack.push(dirRoot);

	totalFilesInTree = 0;
	AddToIgnoreList(".");
	AddToIgnoreList("..");
	AddToIgnoreList("Thumbs.db");

	//if we want to clear the data firs, we must explicitly call it now
	
	vector<vector<string>> childDirs(maxThreads, vector<string>(0));
	vector<DirNode *> curNode(maxThreads,0);
	
	//okay, now we need to make all of our threads
	
	//packaged_task<void(DirNode *, vector<string>&)> task(&DirectoryTree::processSingleDir);
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
			t[i] = thread(&DirectoryTree::processSingleDir, this, curNode[i], std::ref(childDirs[i]));

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
#endif
}


DirNode * DirectoryTree::GetDirNode(string path)
{
	if (*path.rbegin() != SLASH)
		path += SLASH;

	string curPath = dirRoot->ToString();
	size_t find = path.find(curPath);
	
	//make sure the path we are lookging for is within our tree
	if (find == string::npos)
		return 0;

	string shortPath = path.substr(curPath.size());
	string delims;
	delims += SLASH;
	vector<string> tokens = StringUtils::Tokenize(shortPath,delims);

	size_t i = 0;
	
	DirNode *runner = dirRoot;
	
	//make sure the path we want matches the tree root 
	if (!runner->IsPathName(curPath))
		return 0;
	//if so, then start the actual search
	curPath += (SLASH + tokens[i]+ SLASH);

	while (runner != 0)
	{
		runner = runner->GetChildDir(curPath);
		if (runner == 0)
			return 0;

		if (i < tokens.size() - 1)
			i++;
		else
		{
			if (path == runner->ToString())
				return runner;
			return 0;
		}
		curPath += (tokens[i]+ SLASH);
	}
	return 0;
}
void DirectoryTree::DumpTreeToFile(string path)
{
	FILE *pFile;
	string fileName = path;

	fileName += ".txt";
	
	pFile = fopen (fileName.c_str(),"w");

	if (pFile==0)
		return;
	vector<string> pathList;

	DumpTreeToVector(pathList);

	for(size_t i = 0; i < pathList.size(); i++)
		fprintf(pFile,"%s\n", pathList[i].c_str() );
	
	fclose (pFile);
}

void DirectoryTree::DumpTreeToVector(vector<string> &returnVec)
{
	stack<DirNode *> dStack;

	dStack.push(dirRoot);
	while(!dStack.empty())
	{
		DirNode *curNode = dStack.top(); //get the next dr to process
		dStack.pop(); //take it off of stack
		
		if (curNode == 0)
			continue;
		returnVec.push_back(curNode->ToString());
		if (curNode->childDir.empty())
			continue;
		list<DirNode *>::iterator it;
		for (it = curNode->childDir.begin(); it != curNode->childDir.end(); it++)
			dStack.push((*it));
	} 
}


string DirectoryTree::GetRandomFile()
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
long long DirectoryTree::GetDirSize(string path)//needs to have *.fileExt to work
{
	long long size = 0;
	stack<string> dirStack;
	dirStack.push(path);

#ifdef _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

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
				string newPath = curDir + FindFileData.cFileName + SLASH;
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

#else
	DIR *dir = 0;
	struct dirent *ent = 0;
	struct stat buf;
	do{
		string curDir = dirStack.top(); //get the next dr to process
		dirStack.pop(); //take it off of stack

		if (dir != 0)
		{
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != 0)
			{
				if (stat(ent->d_name, &buf))
				{
					if (ent->d_type == DT_REG)
						size += buf.st_size;
				}
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			perror("");
			//return EXIT_FAILURE;
		}
	}while(!dirStack.empty());
#endif
	return size;
}

string DirectoryTree::PrepPathForTraversal(string path)
{
	char ast;
#if _WIN32
	ast = '*';
#else
		ast = ' ';
#endif

	char lastChar = *path.rbegin();
	string newPath = path;
	//if the path is already prepped, then do nothing
	if(lastChar == ast)
		return path;

	else if(lastChar == SLASH)
		newPath += ast;//add asterisk to look for all "files" in cur dir
	else
		newPath += SLASH + ast;//if we forgot to add the last back slash, add it, so we dont crash
	return  newPath;
}
string DirectoryTree::GetFileNameFromPathString(string path)
{
	size_t found = path.find_last_of(SLASH);
	if (found != string::npos)
		return path.substr(found + 1);
	return "";
}

string DirectoryTree::GetPathFromFullyQualifiedPathString(string path)
{
	size_t found = path.find_last_of(SLASH);
	if (found != string::npos)
		return path.substr(0, found);
	return "";
}

void DirectoryTree::ClearAll()
{
	ignoreList.clear();
	totalFilesInTree = 0;
	totalDirsInTree = 0;

	if (dirRoot == 0)
		return;
	
	stack<DirNode *> dStack;
	dStack.push(dirRoot);
	
	while (!dStack.empty())
	{
		DirNode *curNode = dStack.top(); //get the next dr to process
		dStack.pop(); //take it off of stack

		if (curNode == 0)
			continue;

		if (!curNode->childDir.empty())
		{
			list<DirNode *>::iterator it;
			for (it = curNode->childDir.begin(); it != curNode->childDir.end(); it++)
				dStack.push((*it));
		}

		delete curNode;
		curNode = 0;
	}

	//now that everything is deleted, make sure we point the root to 0
	dirRoot = 0;
}
