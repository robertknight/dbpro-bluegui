#include "windows.h"

void debugInt(char* valueInfo, int value)
{
		//256 characters should be enough for anyone ;)

		//Create our new string to hold the str equivilent of the value
		char* strValue=new char[32];
		itoa(value,strValue,10);

		char* newS=new char[80];

		//Copy description into start of string
		strcpy(newS,valueInfo);

		//Add value onto end
		strcat(newS," - ");
		strcat(newS,strValue);
	
		//Show our value to the user
		MessageBox(0,newS,"",0);
		
		//Cleanup
		delete newS;
		delete strValue;
}