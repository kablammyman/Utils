#include <string>
#include "FileStruct.h"
#include "directory.h"

using namespace std;

/*get all the files and folders within this folder, 
and format the strings for later use and printing*/

string DirNode::GetParentDir()
{
	size_t firstSlash = path.rfind(DirectoryTree::SLASH);
	size_t secondSlash = 0;

	//if we dont find anything, we prob have an error
	if (firstSlash == string::npos)
		return "";
	else
	{
		secondSlash = path.rfind(firstSlash, DirectoryTree::SLASH);
		if (secondSlash == string::npos)
		{
			//prob at drive letter, ex: 'C:\'
			return path;
		}
	}
	
	return path.substr (secondSlash,firstSlash);
}
string DirNode::GetFileExt()
{
	size_t dot = path.size()-1;
	while( dot > 0)
	{
		if(path[dot] != '.')
			dot--;

		//if we get to a backslash before a dot, then we are prob looking at at dir
		else if(path[dot] == DirectoryTree::SLASH)
		{
			return "";
		}
		else //we get a dot
		{
			break;
		}
	}

	if(dot == 0)
		return "";
	
	return path.substr (dot+1);//from the dot to the end of the string

}
string DirNode::GetFileName() //seprates the name from the file ext
{
	//start away from end. so we dont "find" the trainling '\\'
	size_t firstSlash = path.rfind(path.size()-2, DirectoryTree::SLASH);
	size_t dot = path.rfind('.');

	//a file without a fileExt...like a vis studio file
	if(dot == string::npos)
	{
		return path.substr (firstSlash);
	}

	return  path.substr (firstSlash,dot);//from start to dot is the name
}


string DirNode::ToString()
{
	string returnString;
	returnString += path;
	
	/*if(!isDir)
	{
		returnString += name;
		returnString += ".";
		returnString += ext;
	}*/
		
	return returnString;
}

string DirNode::FormatPathForPrinting()
{
	string returnString = path;
		
	for(size_t i = 0; i < path.size(); i++)//get rid of the colon and other things
	{
		if(returnString[i] == DirectoryTree::SLASH)
			returnString[i] = '-';
		if(returnString[i] == ':')
			returnString[i] = ' ';
	}
		
	//returnString += SLASH
	/*
	returnString.append(name); 
	if(!isDir)
	{
		returnString += ".";
		returnString += ext;
	}*/
		
	return returnString;
}

DirNode *DirNode::GetChildDir(string dir)
{
	list<DirNode *>::iterator it = childDir.begin();
	while (it != childDir.end())
	{
		if ((*it)->ToString() == dir)
			return *it;
		it++;
	}
	return 0;
}