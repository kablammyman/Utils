// testDirDll.cpp : Defines the entry point for the console application.
//


#include <iostream>

#include "myFileDirDll.h"
#include <string>
#include<vector>

//check execution time
#include <ctime>

using namespace std;

void printTimeStamp(double milis)
{
	int seconds = milis / 1000;
	int minutes = seconds / 60;
	seconds %= 60;
	int hours = minutes / 60;
	minutes %= 60;
	int days = hours / 24;
	hours %= 24;
	cout << days << " days " << hours << " hours " << minutes << " minutes " << seconds << " seconds\n";
}

int main()
{
	//const string dir = "G:\\programming\\win32\\";
	const string dir = "\\\\SERVER\\music";
	//const string dir = "\\\\SERVER\\porn\\porno pics";

    int num = MyFileDirDll::getNumFoldersinDir(dir);
	cout << "num folders in "<< dir<< ":" << num <<endl;

	
	vector<string> folderNames = MyFileDirDll::getAllFolderNamesInDir(dir);
	cout << "\n--------------folders within "<<dir<<":-------------\n";
	for(size_t i = 0; i < folderNames.size(); i++)
		cout << folderNames[i] <<endl;

	num = MyFileDirDll::getNumFilesInDir(dir);
	cout << "num files in "<< dir<< ":" << num <<endl;

	vector<string> fileNames = MyFileDirDll::getAllFileNamesInDir(dir);
	cout << "\n--------------files within "<<dir<<":-------------\n";
	for(size_t i = 0; i < fileNames.size(); i++)
		cout << fileNames[i] <<endl;

	string randDir = MyFileDirDll::getRandomDirQuick(dir);
	cout << "random dir from "<< dir<< ":\n" << randDir <<endl;

	string randFile = MyFileDirDll::getRandomFileQuick(randDir);
	cout << "random file from "<< randDir<< ":\n" << randFile <<endl;

	__int64 size = MyFileDirDll::getDirSize(randDir);
	cout << "dir size of "<< randDir<< " in bytes: "<< size <<endl;


	//MyFileDirDll::deleteFile(string file, bool permanetDelete = false);
	//string MyFileDirDll::deleteAllFilesInDir(string path);

	int start_s = clock();
	cout << "\n--------------now processing "<<dir<<":-------------\n";
	MyFileDirDll::addDirTree(dir,10);
	int stop_s = clock();

	double milis = (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
	printTimeStamp(milis);
	/*double secs = (milis * 0.001);
	double mins = (secs * 0.0166667);
	cout << "milis: " << milis << endl;
	cout << "secs: " << secs << endl;
	cout << "mins: " << mins << endl;*/

	cout << "getDir passed " << MyFileDirDll::test() << " out of 3 test\n";

	
	int numDirs = MyFileDirDll::getNumDirsInTree(dir);
	cout << "num dirs in "<< dir<< ":" << numDirs <<endl;

	int numFiles = MyFileDirDll::getNumFilesInTree(dir);
	cout << "num files in "<< dir<< ":" << numFiles <<endl;

	//vector<string> MyFileDirDll::dumpTreeToVector(bool writeDirOnly = false );

	 
	cout << "\n--------------clearing info from "<<dir<<":-------------\n";
	MyFileDirDll::clearDirTree();

	cout << "\n-------------verify clear:-------------\n";
	numDirs = MyFileDirDll::getNumDirsInTree(dir);
	cout << "num dirs in "<< dir<< ":" << numDirs <<  endl;

	numFiles = MyFileDirDll::getNumFilesInTree(dir);
	cout << "num files in "<< dir<< ":" << numFiles <<endl;

	//try to clear it again, to make sure that we dont have dangling pointers
	MyFileDirDll::clearDirTree();

	cout << "\n--------------now rebuild the tree 1 step at a time " << dir << ":-------------\n";
	MyFileDirDll::startDirTreeStep(dir);

	string temp = MyFileDirDll::nextDirTreeStep();
	cout << "first step has " << temp << endl;
	temp = MyFileDirDll::nextDirTreeStep();
	cout << "second step has " << temp << endl;
	temp = MyFileDirDll::nextDirTreeStep();
	cout << "third step has " << temp << endl;

	/*MyFileDirDll::FileNodeHandle temp = MyFileDirDll::nextDirTreeStep();
	cout << "first step has " << MyFileDirDll::FileNodeHandleToString(temp) << endl;
	temp = MyFileDirDll::nextDirTreeStep();
	cout << "second step has " << MyFileDirDll::FileNodeHandleToString(temp) << endl;
	temp = MyFileDirDll::nextDirTreeStep();
	cout << "third step has " << MyFileDirDll::FileNodeHandleToString(temp) << endl;*/

    return 0;
}
