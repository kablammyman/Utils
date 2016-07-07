// cfgDllTest.cpp : Defines the entry point for the console application.
//
//used with myCfgDll.dll

#include <iostream>

#include "CFGUtils.h"

using namespace std;

int main(int argc, char *argv[])
{
	string temp = argv[0];
	int found = temp.find_last_of("/\\");

	string filePathBase = temp.substr(0, found);
	
	string path = (filePathBase+"\\testConfig.txt");
   // cout << "a + b = " << MathFuncs::MyMathFuncs::Add(a, b) << endl;
   if(!CFGUtils::ReadCfgFile(path,'|'))
   {
	   cout << "couldnt open file: "<<path<<endl;
	   exit(-1);
   }

   bool tempbool = CFGUtils::GetCfgBoolValue("USE_FLOOR_ANIMATION");
   cout << "bool test: " << tempbool <<endl;
   
   int tempint = CFGUtils::GetCfgIntValue("TOP_SPEED");
   cout << "int test: " << tempint <<endl;


   float tempFloat = CFGUtils::GetCfgFloatValue("JUMP_STRENGTH");
   cout << "float test: " << tempFloat <<endl;

   string tempString = CFGUtils::GetCfgStringValue("CATCH_PHRASE");
   cout << "string test: " << tempString <<endl;

   vector<string> simpleList = CFGUtils::GetCfgListValue("ANIMATION_STATES");
   for(size_t i = 0; i < simpleList.size(); i++)
   cout << "simple list item "<< i<< ": " << simpleList[i] <<endl;

   int index = 0;
   cout << "-----now we will parse a complex list: "<<simpleList[index]<<"---\n\n";
   
   int tempint2 = CFGUtils::GetIntValueFromList(simpleList[index],"MOVEMENT_SPEED");
   cout << "complex int test: " << tempint2 <<endl;


   float tempFloat2 = CFGUtils::GetFloatValueFromList(simpleList[index],"ANIMATION_SPEED");
   cout << "complex float test: " << tempFloat2 <<endl;

   string tempString2 = CFGUtils::GetStringValueFromList(simpleList[index],"NAME");
   cout << "complex string test: " << tempString2 <<endl;

   cout << "-----now we will test bad data and such----\n\n";
   //this vlaue doesnt exsit, so what happens?
   string tempString3 = CFGUtils::GetCfgStringValue("iDontExist");
   cout << "value doesnt exist check: " << tempString3 <<endl;

   int min = CFGUtils::GetFloatValueFromList("stopWatch", "min");
   float sec = CFGUtils::GetFloatValueFromList("stopWatch", "sec");
   double mil = CFGUtils::GetFloatValueFromList("stopWatch", "mil");

   //this value is the wrong type, so what happens?
   int tempInt3 = CFGUtils::GetCfgIntValue("stringValue");
   cout << "wrong value check: " << tempInt3 <<endl;

   vector<string> simpleList3 = CFGUtils::GetCfgListValue("FAKE LIST");
   for(size_t i = 0; i < simpleList3.size(); i++)
   cout << "simple list item "<< i<< ": " << simpleList3[i] <<endl;

    return 0;
}

