#include "FileUtils.h"
#include "directory.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <stack>



static DirectoryTree dirTree;//can't use it as a member variable due to the static conflict

int GetRandomNum(int min, int max)
{
	int diff = ((max - min) + 1);
	return ((diff * rand()) / RAND_MAX) + min;
}

//--------------------------------------------------------------------------------------------------	
bool FileUtils::DoesPathExist(string path)
{
#ifdef _WIN32	
	DWORD ftyp = GetFileAttributesA(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!
	return true;
#else
	if(path.back() == '\r')
		path.pop_back();

	DIR* dir = opendir(path.c_str());
	if (dir)
	{
	    /* Directory exists. */
	    closedir(dir);
		return true;
	}
	/*else if (ENOENT == errno)
	{
	    // Directory does not exist. 
		return false;
	}
	else
	{
	    // opendir() failed for some other reason. 
	}*/
	return false;
#endif
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
	if (name == "")//if we passed in a file name without path
		name = fullPath;
	std::size_t found = name.find_last_of(".");
	return name.substr(0,found);
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumFoldersinDir(string path)//needs to have *.fileExt to work
{
	if (DoesPathExist(path) == false)
		return -1;
	int folderNum = 0;
	string curDir = DirectoryTree::PrepPathForTraversal(path);
#if _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
		do {
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)//ignore anything we put in this list
				continue;

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				folderNum++;


		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
#else
	if(path.back() == '\r')
		path.pop_back();

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path.c_str()); /* There should be error handling after this */
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_DIR) { /* If the entry is a regular file */
		 folderNum++;
	    }
	}
	closedir(dirp);

#endif
	return folderNum;
}
//--------------------------------------------------------------------------------------------------
vector<string> FileUtils::GetAllFolderNamesInDir(string path)//needs to have *.fileExt to work
{
	vector<string> folderList;
	if (DoesPathExist(path) == false)
		return folderList;
	string curDir = DirectoryTree::PrepPathForTraversal(path);
	string sTmp;

#if _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	
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
				strcat(sTmp, SLASH);*/
				sTmp = curDir;
				sTmp += FindFileData.cFileName;
				sTmp += SLASH;
				folderList.push_back(sTmp);
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
#else
	if(path.back() == '\r')
		path.pop_back();

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path.c_str());
	while ((entry = readdir(dirp)) != NULL) 
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)//ignore anything we put in this list
			continue;		
		if (entry->d_type == DT_DIR) 
		{ 
			sTmp = curDir;
			sTmp += entry->d_name;
			sTmp += SLASH;
			folderList.push_back(sTmp);
		}
	}
	closedir(dirp);

#endif
	return folderList;
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumFilesInDir(string path, string ext)//needs to have *.fileExt to work
{
	if (DoesPathExist(path) == false)
		return-1;
	string curDir = DirectoryTree::PrepPathForTraversal(path);
	int fileNum = 0;

#if _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	
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
#else
	if(path.back() == '\r')
		path.pop_back();

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path.c_str()); /* There should be error handling after this */
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) { /* If the entry is a regular file */
		 fileNum++;
	    }
	}
	closedir(dirp);
#endif
		return fileNum;
}

//--------------------------------------------------------------------------------------------------
vector<string> FileUtils::GetAllFileNamesInDir(string path,string ext, bool includePath)
{
	vector<string> fileList;
	if (DoesPathExist(path) == false)
		return fileList;
	string curDir = DirectoryTree::PrepPathForTraversal(path);
	string sTmp = "";

#if _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	
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
#else
	if(path.back() == '\r')
		path.pop_back();

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path.c_str());
	while ((entry = readdir(dirp)) != NULL) 
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)//ignore anything we put in this list
			continue;		
		if (entry->d_type == DT_REG) 
		{ 
			sTmp = curDir;
			sTmp += entry->d_name;
			sTmp += SLASH;
			fileList.push_back(sTmp);
		}
	}
	closedir(dirp);
#endif
		return fileList;
}
//--------------------------------------------------------------------------------------------------
bool FileUtils::Delete_File(string file, bool permanetDelete)
{
#if _WIN32
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
#else
	if (remove(file.c_str()) != 0)
		return false;
	return true;
#endif
}
//--------------------------------------------------------------------------------------------------
string FileUtils::DeleteAllFilesInDir(string path)
{

	string returnString = "";

	vector<string> allFiles = GetAllFileNamesInDir(path, "", true);

	for (size_t i = 0; i < allFiles.size(); i++)
	{
		if (Delete_File(allFiles[i]) == false)
		{
			returnString += "Error deleting file: ";
			returnString += allFiles[i];
			returnString += "\n";
		}
	}
	
	return returnString;
}
//--------------------------------------------------------------------------------------------------------
//pics a dir from the master list, then "digs" down to get soething new
string FileUtils::GetRandomDirQuick(string path)
{
	if (DoesPathExist(path) == false)
		return "";

	//int curDepth = 0;
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
	vector<string> allFiles = GetAllFileNamesInDir(path, "", true);

	if (allFiles.size() > 0)
	{
		//used to crash here
		int num = GetRandomNum(0, (int)allFiles.size() - 1);
		return allFiles[num];
	}
	return ""; //this dir was empty, return empty string
}
//--------------------------------------------------------------------------------------------------------
string FileUtils::GetFileNameFromPathString(string path)
{
	return DirectoryTree::GetFileNameFromPathString(path);
}
//--------------------------------------------------------------------------------------------------------
string FileUtils::GetPathFromFullyQualifiedPathString(string path)
{
	return DirectoryTree::GetPathFromFullyQualifiedPathString(path);
}
//--------------------------------------------------------------------------------------------------
void FileUtils::AddDirTree(string path, int numThreads)
{
	if (DoesPathExist(path) == false)
		return;
	if(numThreads == 0)
		dirTree.ProcessFilesFromDir(path);
	else
		dirTree.ProcessFilesFromDirMT(path, numThreads);
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
	return  dirTree.GetNumFilesInTree();
}
//--------------------------------------------------------------------------------------------------
void FileUtils::StartDirTreeStep(string path)
{
	dirTree.StartProcessFilesFromDirStep(path);
}
//--------------------------------------------------------------------------------------------------
/*DirNode* FileUtils::nextDirTreeStep()
{
	return dirTree.nextProcessFilesFromDirStep();
}*/
//--------------------------------------------------------------------------------------------------
string FileUtils::NextDirTreeStep()
{
	DirNode* temp = dirTree.NextProcessFilesFromDirStep();
	return temp->ToString();
}
//--------------------------------------------------------------------------------------------------
size_t FileUtils::GetCurNodeNumFiles()
{
	DirNode* temp = dirTree.GetcurNodeInStep();
	if (temp != 0)
		return temp->GetNumFiles();
	return 0;
}
//--------------------------------------------------------------------------------------------------
size_t FileUtils::GetCurNodeNumFolders()
{
	DirNode* temp = dirTree.GetcurNodeInStep();
	if(temp != 0)
		return temp->GetNumFolders();
	return 0;
}
//--------------------------------------------------------------------------------------------------
list<string> FileUtils::GetCurNodeFileList()
{
	DirNode* temp = dirTree.GetcurNodeInStep();
	if (temp != 0)
		return temp->fileList;
	list<string> empty;
	return empty;
}
//--------------------------------------------------------------------------------------------------
bool FileUtils::IsFinished()
{
	return dirTree.IsFinihsedProcessing();
}
//--------------------------------------------------------------------------------------------------
void FileUtils::ClearDirTree()
{
	dirTree.ClearAll();
}
//--------------------------------------------------------------------------------------------------
int FileUtils::GetNumDirsInTree(string path)
{
	return dirTree.GetNumDirsInTree();
}

//--------------------------------------------------------------------------------------------------
void FileUtils::DumpTreeToVector(string path, vector<string> &ret)
{
	dirTree.DumpTreeToVector(ret);
}
//--------------------------------------------------------------------------------------------------
/*string FileUtils::FileNodeHandleToString(FileNodeHandle dirNode)
{
	return dirNode->ToString();
}*/
//--------------------------------------------------------------------------------------------------
long long FileUtils::GetDirSize(string path)
{
	return DirectoryTree::GetDirSize(path);
}

int FileUtils::Test()
{
	//dirTree.dumpTreeToFile("G:\\programming\\my tools\\FileUtils\\testFile.txt");

	string badPath = "\\\\SERVER\\music\\2pac";
	string goodPath = "\\\\SERVER\\music\\muzik\\2pac";
	DirNode *dontExist = dirTree.GetDirNode(badPath);
	if (dontExist != 0)
		return 0;
	DirNode *iExist = dirTree.GetDirNode(goodPath);
	if (iExist == 0)
		return 1;
	goodPath += SLASH;

	if (iExist->ToString() == goodPath)
		return 3;

	return 2;
}
