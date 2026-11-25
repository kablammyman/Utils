#include "FileUtils.h"
#include "directory.h"
#include "StringUtils.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <random>
#include <time.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <stack>



static DirectoryTree dirTree;//can't use it as a member variable due to the static conflict

//---------------------------------------------------------------------------------------
int FileUtils::GetRandomInt(int min, int max)
{

	
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);

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
	if(path.empty())
		return false;
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
//--------------------------------------------------------------------------------------------------
bool FileUtils::DoesFileExist(string path)
{
    if (FILE *file = fopen(path.c_str(), "r"))
	{
        fclose(file);
        return true;
    }
	else
        return false;
}
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
	//only a dot in the file name counts (a folder like "St. Francis" isnt an extension), none = ""
	std::size_t slash = fullPath.find_last_of("/\\");
	std::size_t found = fullPath.find_last_of(".");
	if (found == string::npos || (slash != string::npos && found < slash))
		return "";
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
#ifndef _WIN32
//d_type is DT_UNKNOWN on some filesystems (xfs/nfs/cifs) and DT_LNK for symlinks; stat() tells us what it really is
static unsigned char DirEntryType(const string &dir, struct dirent *entry)
{
	if (entry->d_type != DT_UNKNOWN && entry->d_type != DT_LNK)
		return entry->d_type;
	string full = dir;
	if (!full.empty() && full.back() != '/')
		full += '/';
	full += entry->d_name;
	struct stat st;
	if (stat(full.c_str(), &st) != 0)
		return DT_UNKNOWN;
	return S_ISDIR(st.st_mode) ? DT_DIR : S_ISREG(st.st_mode) ? DT_REG : DT_UNKNOWN;
}
#endif
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
	if (dirp == NULL) //no permission, or deleted since DoesPathExist
		return folderList;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)//ignore anything we put in this list
			continue;
		if (DirEntryType(path, entry) == DT_DIR)
		{
			sTmp = curDir;
			//(this used to strip a leading space with temp.erase(temp[0]), which actually cut the name to 32 chars,
			//and the path wouldnt exist without the space anyway)
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
vector<string> FileUtils::GetAllFileNamesInDir(string path)
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
				sTmp.clear();
				sTmp += FindFileData.cFileName;

				fileList.push_back(sTmp);
			}

		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
#else
	//same as windows: just the file names (no path), no folders
	if(!path.empty() && path.back() == '\r')
		path.pop_back();

	DIR * dirp = opendir(path.c_str());
	if (dirp == NULL)
		return fileList;
	struct dirent * entry;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		if (DirEntryType(path, entry) == DT_REG)
			fileList.push_back(entry->d_name);
	}
	closedir(dirp);
#endif
	return fileList;
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
	if(!path.empty() && path.back() == '\r')
		path.pop_back();

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path.c_str());
	if (dirp == NULL)
		return fileList;
	while ((entry = readdir(dirp)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)//ignore anything we put in this list
			continue;
		//only files with this extension, like windows does (linux used to return every file).
		//note: linux always returns the full path, whatever includePath says (callers rely on that)
		if (DirEntryType(path, entry) == DT_REG && (ext == "" || GetFileExt(entry->d_name) == ext))
		{
			sTmp = curDir;
			sTmp += entry->d_name;
			//sTmp += SLASH;
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
		vector<string> curDirList = GetAllFolderNamesInDir(curDir);

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
				nextDir = GetRandomInt(0, (int)curDirList.size() - 1);
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
		int num = GetRandomInt(0, (int)(allFiles.size() - 1));
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
int FileUtils::GetNumFilesInTree()
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
int FileUtils::GetNumDirsInTree()
{
	return dirTree.GetNumDirsInTree();
}

//--------------------------------------------------------------------------------------------------
void FileUtils::DumpTreeToVector(vector<string> &ret)
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
//--------------------------------------------------------------------------------------------------

bool FileUtils::CreateNewDir(string path)
{
#if _WIN32
	string strCommand = "if not exist \"" + path + "\" mkdir \"" + path +"\"";
	int iResult = system(strCommand.c_str());
	if (iResult != 0)
	{
		//printf("Error: couldn't create directory (%s)\n", path);
		return false;
	}
	return true;
#else
	//make each folder on the way down, no shell. (this used to run "mkdir - p", which made folders called
	//"-" and "p" in the current dir, couldnt make parents, and let a " or $ in the path run commands)
	if (path.empty())
		return false;
	for (size_t i = 1; i <= path.size(); i++)
	{
		if (i < path.size() && path[i] != '/')
			continue;
		string part = path.substr(0, i);
		if (mkdir(part.c_str(), 0775) != 0 && errno != EEXIST)
			return false;
	}
	struct stat st;
	return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

//--------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------------------------------
bool FileUtils::CreateNewFileFromTagTemaplateFile(string templatePath, string newFilePath, DocumentTags tagMap)
{

	//if not os.path.exists(outputFolderBase):
	//	os.makedirs(outputFolderBase)

	ifstream templateFile;
	ofstream newFile;

	templateFile.open(templatePath, std::fstream::in);
	newFile.open(newFilePath, std::fstream::out);

	if (!templateFile.is_open())
	{
		templateFile.close();
		printf("CreateNewFileFromTagTemaplateFile error 1: couldnt open template: %s\n", templatePath.c_str());
		return false;
	}

	//when you ha e 2 paercels in one deal, this needs to change, otherwise you get this:
	//\\SERVER\documents\land investing\parcels\Texas\Henderson\R000054401 R000054402\Documents\sellDeed_Frances A. Moinet.rtf
	if (!newFile.is_open())
	{
		newFile.close();
		printf("CreateNewFileFromTagTemaplateFile error 2: couldnt open new file: %s\n", newFilePath.c_str());
		return false;
	}

	string line;
	int wordListIndex = 0;
	while (getline(templateFile, line))
	{
		string newLine = StringUtils::ReplaceTagForValue(line, tagMap);
		newFile << newLine << endl;
	}

	templateFile.close();
	newFile.close();
	return true;
}
