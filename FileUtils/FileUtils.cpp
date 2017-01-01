#include "FileUtils.h"
#include "directory.h"

#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <stack>



static directoryTree dirTree;//can't use it as a member variable due to the static conflict

int GetRandomNum(int min, int max)
{
	int diff = ((max - min) + 1);
	return ((diff * rand()) / RAND_MAX) + min;
}
//-----------------------------------------
//use only with path to folders, not files!
string PrepPathForTraversal(string path)
{
	char lastChar = *path.rbegin();
	string newPath = path;
	if (lastChar == '\\')
		newPath.append("*");//add asterisk to look for all "files" in cur dir
	else
		newPath.append("\\*");//if we forgot to add the last back slash, add it, so we dont crash
	return  newPath;
}
//--------------------------------------------------------------------------------------------------	
bool FileUtils::DoesPathExist(string path)
{
	DWORD ftyp = GetFileAttributesA(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!
	return true;
}
/* rename example */

//--------------------------------------------------------------------------------------------------	
bool FileUtils::Move_File(string oldName, string newName)
{
	//should i do this one first?
	//BOOL WINAPI MoveFile( lpExistingFileName,LPCTSTR lpNewFileName);

	int result = rename(oldName.c_str(), newName.c_str());
	if (result == 0)
		return true;
	else
		return false;
}
//--------------------------------------------------------------------------------------------------
string FileUtils::GetFileExt(string fullPath)
{
	std::size_t found = fullPath.find_last_of(".");
	return fullPath.substr(found + 1);
}
//--------------------------------------------------------------------------------------------------
string FileUtils::GetFileNameNoExt(string fullPath)
{
	string name = GetFileNameFromPathString(fullPath);
	std::size_t found = name.find_last_of(".");
	return name.substr(0,found);
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumFoldersinDir(string path)//needs to have *.fileExt to work
{
	if (DoesPathExist(path) == false)
		return -1;

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);

	int folderNum = 0;

	//	curDir +="*";//add asterisk to look for all "files" in cur dir
	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				folderNum++;


		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);

		return folderNum;
}
//--------------------------------------------------------------------------------------------------
vector<string> FileUtils::GetAllFolderNamesInDir(string path)//needs to have *.fileExt to work
{
	vector<string> folderList;
	if (DoesPathExist(path) == false)
		return folderList;

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);
	//char sTmp[MAX_PATH]="\0";
	string sTmp;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				/*strcpy(sTmp, curDir.c_str());
				strcat(sTmp, FindFileData.cFileName);//cur dir + new folder name = new dir to explre
				strcat(sTmp, "\\");*/
				sTmp = curDir;
				sTmp += FindFileData.cFileName;
				sTmp += "\\";
				folderList.push_back(sTmp);
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);

		return folderList;
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumFilesInDir(string path, string ext)//needs to have *.fileExt to work
{
	if (DoesPathExist(path) == false)
		return-1;

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);
	int fileNum = 0;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (ext == "")
					fileNum++;
				else if (GetFileExt(FindFileData.cFileName) == ext)
					fileNum++;
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);

		return fileNum;
}

//--------------------------------------------------------------------------------------------------
vector<string> FileUtils::GetAllFileNamesInDir(string path,string ext, bool includePath)//needs to have *.fileExt to work
{
	vector<string> fileList;
	if (DoesPathExist(path) == false)
		return fileList;

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);
	//char sTmp[MAX_PATH]="\0";
	string sTmp = "";

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//strcpy(sTmp, curDir.c_str());
				//strcat(sTmp, FindFileData.cFileName);//cur dir + new folder name = new dir to explre
				if(includePath)
					sTmp = curDir;
				else
					sTmp.clear();
				sTmp += FindFileData.cFileName;

				if(ext == "")
					fileList.push_back(sTmp);
				
				else if(GetFileExt(FindFileData.cFileName) == ext)
					fileList.push_back(sTmp);
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);

		return fileList;
}
//--------------------------------------------------------------------------------------------------
bool FileUtils::Delete_File(string file, bool permanetDelete)
{
	if (DoesPathExist(file) == false)
		return false;

	if (permanetDelete)
	{
		if (DeleteFile(file.c_str()))
			return true;
		return false;
	}
	file += '\0';
	file += '\0';
	SHFILEOPSTRUCT operation;
	ZeroMemory(&operation, sizeof(operation));

	operation.wFunc = FO_DELETE;
	operation.pFrom = file.c_str();//pathToFile;//"c:\\file\to\delete.txt";
	operation.fFlags = FOF_ALLOWUNDO | FOF_NO_UI | FOF_NORECURSION;

	if (SHFileOperation(&operation) == 0)
		return true;
	return false;
}
//--------------------------------------------------------------------------------------------------
string FileUtils::DeleteAllFilesInDir(string path)
{
	if (DoesPathExist(path) == false)
		return "file path does not exist";

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);

	string returnString = "";

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			else
			{
				string file = curDir + FindFileData.cFileName;
				if (Delete_File(file) == false)
				{
					returnString += "Error deleting file: ";
					returnString += file;
					returnString += "\n";
				}
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		return returnString;
}
//--------------------------------------------------------------------------------------------------------
//pics a dir from the master list, then "digs" down to get soething new
string FileUtils::GetRandomDirQuick(string path, bool useIgnoreList)
{
	if (DoesPathExist(path) == false)
		return "";

	int curDepth = 0;
	string curDir = path;
	bool done = false;
	int loopCount = 0;
	vector<string> digPath; //to keep track of the way we get to our final folder

	while (!done)//keep "digging" until theres no more folders, just images remain
	{
		vector<string> curDirList = GetAllFolderNamesInDir(curDir.c_str());

		if (curDirList.size() == 0)//there are no more folders
			done = true;
		else if (loopCount > 10)
		{
			curDir = "";//return an empty string, since no valid directorys were "found" (more like stumbled upon)
			done = true;
		}

		else
		{
			int nextDir = 0;
			if (curDirList.size() - 1 > 1)
				nextDir = GetRandomNum(0, (int)curDirList.size() - 1);
			string newDir = curDirList[nextDir];

			curDir = newDir;//no directory is off limits

		}
	}
	return curDir;
}
//--------------------------------------------------------------------------------------------------------
//enumerates all files, and pics a random one
string FileUtils::GetRandomFileQuick(string path)
{
	if (DoesPathExist(path) == false)
		return "";

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = PrepPathForTraversal(path);

	vector<string> files;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	size_t removeAst = curDir.length() - 1;
	curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			else
			{
				files.push_back(curDir + FindFileData.cFileName);
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		if (files.size() > 0)
		{
			//used to crash here
			int num = GetRandomNum(0, (int)files.size() - 1);
			return files[num];
		}
		return ""; //this dir was empty, return empty string
}
//--------------------------------------------------------------------------------------------------------
string FileUtils::GetFileNameFromPathString(string path)
{
	return directoryTree::getFileNameFromPathString(path);
}
//--------------------------------------------------------------------------------------------------------
string FileUtils::GetPathFromFullyQualifiedPathString(string path)
{
	return directoryTree::getPathFromFullyQualifiedPathString(path);
}
//--------------------------------------------------------------------------------------------------
void FileUtils::AddDirTree(string path, int numThreads)
{
	if (DoesPathExist(path) == false)
		return;
	if(numThreads == 0)
		dirTree.processFilesFromDir(path);
	else
		dirTree.processFilesFromDirMT(path, numThreads);
}
//--------------------------------------------------------------------------------------------------
/*DirNode* FileUtils::getDirTreeRoot()
{
	return dirTree.getDirTreeRoot();
}
//--------------------------------------------------------------------------------------------------
DirNode* FileUtils::getDirTree(string path)
{
	return dirTree.getDirNode(path);
}*/
//--------------------------------------------------------------------------------------------------
/*int FileUtils::getNumFilesInTree(FileNodeHandle dirNode)
{
	dirNode
}*/
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumFilesInTree(string path)
{
	return  dirTree.getNumFilesInTree();
}
//--------------------------------------------------------------------------------------------------
void FileUtils::StartDirTreeStep(string path)
{
	dirTree.startProcessFilesFromDirStep(path);
}
//--------------------------------------------------------------------------------------------------
/*DirNode* FileUtils::nextDirTreeStep()
{
	return dirTree.nextProcessFilesFromDirStep();
}*/
//--------------------------------------------------------------------------------------------------
string FileUtils::NextDirTreeStep()
{
	DirNode* temp = dirTree.nextProcessFilesFromDirStep();
	return temp->ToString();
}
//--------------------------------------------------------------------------------------------------
size_t FileUtils::GetCurNodeNumFiles()
{
	DirNode* temp = dirTree.getcurNodeInStep();
	if (temp != NULL)
		return temp->GetNumFiles();
	return -1;
}
//--------------------------------------------------------------------------------------------------
size_t FileUtils::GetCurNodeNumFolders()
{
	DirNode* temp = dirTree.getcurNodeInStep();
	if(temp != NULL)
		return temp->GetNumFolders();
	return -1;
}
//--------------------------------------------------------------------------------------------------
list<string> FileUtils::GetCurNodeFileList()
{
	DirNode* temp = dirTree.getcurNodeInStep();
	if (temp != NULL)
		return temp->fileList;
	list<string> empty;
	return empty;
}
//--------------------------------------------------------------------------------------------------
bool FileUtils::IsFinished()
{
	return dirTree.isFinihsedProcessing();
}
//--------------------------------------------------------------------------------------------------
void FileUtils::ClearDirTree()
{
	dirTree.clearAll();
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumDirsInTree(string path)
{
	return dirTree.getNumDirsInTree();
}

//--------------------------------------------------------------------------------------------------
void FileUtils::DumpTreeToVector(string path, vector<string> &ret, bool writeDirOnly)
{
	dirTree.dumpTreeToVector(ret);
}
//--------------------------------------------------------------------------------------------------
/*string FileUtils::FileNodeHandleToString(FileNodeHandle dirNode)
{
	return dirNode->ToString();
}*/
//--------------------------------------------------------------------------------------------------
__int64 FileUtils::GetDirSize(string path)
{
	if (DoesPathExist(path) == false)
		return -1;

	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	//char sTmp[MAX_PATH]="\0";
	string sTmp;
	__int64 size = 0;

	stack<string> dirStack;
	dirStack.push(path);

	do {
		string curDir = PrepPathForTraversal(dirStack.top()); //get the next dir to process
		dirStack.pop(); //take it off of stack

		hFind = FindFirstFile(curDir.c_str(), &FindFileData);

		size_t removeAst = curDir.length() - 1;
		curDir.resize(removeAst); //curDir[removeAst]='\0';//remove the asterisk

		if (hFind != INVALID_HANDLE_VALUE)
			do {

				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
					continue;

				//if we find a directory, add its name to the stack, so we can parse it later
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					sTmp = curDir;
					sTmp += FindFileData.cFileName;
					sTmp += "\\";
					dirStack.push(sTmp);
				}
				//else this is a file, so get file size and add to total
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

			} while (FindNextFile(hFind, &FindFileData));

	} while (!dirStack.empty());

	FindClose(hFind);
	return size;
}

int FileUtils::Test()
{
	//dirTree.dumpTreeToFile("G:\\programming\\my tools\\FileUtils\\testFile.txt");

	string badPath = "\\\\SERVER\\music\\2pac";
	string goodPath = "\\\\SERVER\\music\\muzik\\2pac";
	DirNode *dontExist = dirTree.getDirNode(badPath);
	if (dontExist != NULL)
		return 0;
	DirNode *iExist = dirTree.getDirNode(goodPath);
	if (iExist == NULL)
		return 1;
	goodPath += '\\';

	if (iExist->ToString() == goodPath)
		return 3;

	return 2;
}
