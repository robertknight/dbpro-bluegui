#include "GUI-DLL.h"
#include <Shlobj.h>
#include "dialogs.h"

HWND dialogOwnerWindow=0;
char multiFileSeparator=',';

/*  
	COMMAND_HELP

	setDialogOptions int setting, int value

	Changes additional settings for BlueGUI dialogs.

	Currently the only setting that can be changed through this command is the character which separates 
	each filename when the user selects multiple files in an open file dialog.

	setting:  The setting to change, possible values:
			1 - Set separator between each filename when multiple files are returned.  value should be set to the 
				ASCII character code of the desired character (which is a comma by default).  You can get this value
				using the asc function (eg. setDialogOptions 1,asc("|")  would change the character to a pipe.)
	value:  The value for the new setting.
*/
EXPORT void setDialogOptions(int setting, int value)
{
	enum DialogOptions
	{
		FileSeparator=1
	};

	switch (setting)
	{
	case FileSeparator:
		multiFileSeparator=(char)value;
		break;
	}
}

EXPORT void message(char* caption,char* messageText)
{
	MessageBox(dialogOwner(),messageText,caption,0);
	  
}

EXPORT void errorMessage(char* messageText)
{
	MessageBox(dialogOwner(),messageText,"Error",MB_ICONERROR);
}

EXPORT void warningMessage(char* messageText)
{
	MessageBox(dialogOwner(),messageText,"Warning",MB_ICONEXCLAMATION);
}

EXPORT int questionMessage(char* messageText, char* caption)
{
	int result=MessageBox(dialogOwner(),messageText,caption,MB_YESNOCANCEL);

	if (result==IDYES)
	{
		
		return 1;
	}
	else
	{
		if (result==IDNO)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

EXPORT void messageNoCaption(char* messageText)
{
	MessageBox(dialogOwner(),messageText,"Information",0);
}

HWND dialogOwner()
{
	
	if (IsWindowVisible(dialogOwnerWindow))
	{
		return dialogOwnerWindow;
	}
	else
	{
		return 0;
	}
}

char* fileDialog(OPENFILENAME& dialog, char* caption, char* tempFileTypes, int options, char* initDir)
{
	
	ZeroMemory(&dialog,sizeof(OPENFILENAME));

	const int MaxFileNameLength=2000;
	
	DWORD fileTypeLen=strlen(tempFileTypes);
	char* fileName=new char[MaxFileNameLength];

	//Because of DBPro's string system, we cannot safely modify input strings.
	//Therefore we have to create a temporary string and pass that to the GetOpen/SaveFileName functions
	char* fileTypes=new char[fileTypeLen+2]; //Must be deleted afterwards
	
	strcpy(fileTypes,tempFileTypes);

	*(fileName)=0;

	//Loop through fileTypes string and replace pipe characters with null characters.
	//I chose to use pipe characters to split up the fileTypes string as its easier for the user,
	//eg: "Text Files (*.txt)|*.txt|All Files (*.*)|*.*"
	for (DWORD i=0;i<fileTypeLen;i++)
	{
		if (*(fileTypes+i)==124) //124 is ASCII code for pipe character
		{
			*(fileTypes+i)=0; //0 is ASCII code for null character
		}
	}

	*(fileTypes+fileTypeLen)=0;
	*(fileTypes+fileTypeLen+1)=0;


	dialog.lpstrTitle=caption;
	dialog.lpstrFilter=fileTypes;

	
	dialog.Flags=0;

	//Check user specified dialog options

	//Multiple File Selection
	if (options & 1)
	{
		dialog.Flags |= OFN_ALLOWMULTISELECT;
		dialog.Flags |= OFN_EXPLORER; //Important - must set this or we get an old-fasioned Windows 3.1 style dialog
	}
	
	//dialog.FlagsEx=0;
	dialog.hInstance=CApplication::core->hInstance;
	dialog.hwndOwner=dialogOwner();
	dialog.lCustData=0;
	dialog.lpfnHook=0;
	dialog.lpstrCustomFilter=0;
	dialog.lpstrDefExt=0;
	dialog.lpstrFile=fileName;
	dialog.lpstrFileTitle=0;
	dialog.lpstrInitialDir=initDir;
	dialog.lpTemplateName=0;
	dialog.lStructSize=sizeof(OPENFILENAME);
	dialog.nFileExtension=0;
	dialog.nFileOffset=0;
	dialog.nFilterIndex=0;
	dialog.nMaxCustFilter=0;
	dialog.nMaxFile=MaxFileNameLength;
	dialog.nMaxFileTitle=MaxFileNameLength;
	
	//dialog.dwReserved=0;
	//dialog.pvReserved=0;

	//Finally return a pointer to the filename string
	return fileName;

}

EXPORT DWORD saveDialog_noInitDir(DWORD oldString, char* caption, char* fileTypes)
{
	return saveDialog(oldString,caption,fileTypes,0);
}

EXPORT DWORD openDialog_noInitDir(DWORD oldString, char* caption, char* fileTypes, int options)
{
	return openDialog(oldString,caption,fileTypes,options,0);
}

EXPORT DWORD openDialog_noOptions(DWORD oldString, char* caption, char* fileTypes)
{
	return openDialog(oldString,caption,fileTypes,0,0);
}

EXPORT DWORD openDialog(DWORD oldString, char* caption, char* fileTypes, int options, char* initDir)
{
	deleteString(oldString);

	//Preserve current directory by retrieving it now and then resetting it at the end
	int length=GetCurrentDirectory(0,0);
	char* currentDir=new char[length];
	GetCurrentDirectory(length,currentDir);

	//Create dialog structure
	OPENFILENAME dialog;

	//Prepares the dialog structure
	char* fileName=fileDialog(dialog, caption, fileTypes, options, initDir); 

	if (GetOpenFileName(&dialog)==TRUE)
	{
	
	//If multiple file selection is enabled, the string will be in the format
	//[FolderName] [Null] [File1] [Null] [File2] ... [Null] [Null]
	//this needs to be converted to a string in the format
	//[FolderName] [Comma] [File1] [Comma] [File2]

	if (options & 1)
	{
		bool foundEnd=false;
		int len=0;

		//Replace all single nulls with a comma
		while (!foundEnd)
		{
			if (*(fileName+len)==0)
			{
				//Found null char, replace with comma
				*(fileName+len)=multiFileSeparator;
			}
			
			len++;
			if ((*(fileName+len)==0) && (*(fileName+len+1)==0))
			{
				foundEnd=true;
			}
		}
	}
		
		SetCurrentDirectory(currentDir);
		delete currentDir;

		DWORD outString=returnString(fileName);
		return outString;
	}
	else
	{
		SetCurrentDirectory(currentDir);
		delete currentDir;

		return 0;
	}
}

EXPORT DWORD saveDialog(DWORD oldString, char* caption, char* fileTypes, char* initDir)
{
	
	deleteString(oldString);
	

	//Preserve current directory by getting it now and resetting it at the end
	int length=GetCurrentDirectory(0,0);
	char* currentDir=new char[length];
	GetCurrentDirectory(length,currentDir);

	//Create dialog structure
	OPENFILENAME	dialog;

	//Parse fileTypes and produce array of extensions
	DWORD			fileTypeLen=strlen(fileTypes);
	
	char*			extensionsArray=new char[fileTypeLen];
	ZeroMemory(extensionsArray,fileTypeLen);
	bool			recordChar=false;
	DWORD			eaPos=0;

	//Search through fileTypes string and produce a new string with just the extensions in, seperated
	//by pipe characters
	for (DWORD i=0;i<fileTypeLen;i++)
	{
		if (*(fileTypes+i)==124) //124 = Pipe Character
		{
			//Change from a "Description" part of the file-types string to an "Extension" part, or vice-versa
			//Each part is seperated by a pipe character
			recordChar=!recordChar;

			if (recordChar)
			{
				*(extensionsArray+eaPos)=124;
				eaPos++;
			}
		}
		else
		{
			//If we are in a file-extension part of the file types string (rather than the description bit)
			//then copy the character to the extensionsArray string, which we can later parse when we get the 
			//extension index from the save dialog
			if (recordChar)
			{
				*(extensionsArray+eaPos)=*(fileTypes+i);
				eaPos++;
			}
		}
	}
	//Add final ending pipe char to signify end of string
	*(extensionsArray+eaPos)=124;

	//Add terminating null character
	*(extensionsArray+eaPos+1)=0;

	DWORD eaLen=eaPos+1;
	DWORD splittersFound=0;
	DWORD startPos=0;
	DWORD endPos=0;

	

	//Prepares dialog structure
	char* fileName=fileDialog(dialog, caption, fileTypes, 0, initDir);

	if (GetSaveFileName(&dialog)==TRUE)
	{
	//	delete dialog.lpstrFilter;

		DWORD fileNameLen=strlen(fileName);

		//Add file extension if not specified
		if (!strrchr(fileName,46))
		{
			//Find the starting character of the extension
			for (DWORD i=0;i<eaLen;i++)
			{
				if (*(extensionsArray+i)==46)
				{
					splittersFound++;
					//dialog.nFilterIndex is the FileTypes combobox index.  Loop through each character
					//in the string until we have encountered the correct number of "splitters", which tells
					//us that we are at the start of the right extension
					if (splittersFound==dialog.nFilterIndex)
					{
						startPos=i;
					}
				}
			}

			//Find the ending character of the extension
			endPos=startPos;
			while (*(extensionsArray+endPos) != 124) //124 = Pipe character (start of next section)
			{
				endPos++;
			}

			//Copy the file extension characters into the filename
			for (i=0;i<(endPos-startPos);i++)
			{
				*(fileName+fileNameLen+i)=*(extensionsArray+startPos+i);
			}

			//Null terminate the filename
			*(fileName+fileNameLen+(endPos-startPos))=0;
		}

		//Delete the extensions string and return
		delete extensionsArray;

		SetCurrentDirectory(currentDir);
		delete currentDir;

		DWORD outString=returnString(fileName);
		return outString;
	}
	else
	{
//WIN98FIX
//		delete dialog.lpstrFilter;

		SetCurrentDirectory(currentDir);
		delete currentDir;

		delete extensionsArray;
		return 0;
	}
	
}

EXPORT DWORD colorDialog()
{
	static COLORREF customColors[16];

	LPCHOOSECOLOR colorInfo=new CHOOSECOLOR;
	colorInfo->Flags=CC_ANYCOLOR;
	colorInfo->hInstance=0;

	colorInfo->hwndOwner=dialogOwner();
//#endif
	colorInfo->lCustData=0;
	colorInfo->lpCustColors=(COLORREF*)customColors;
	colorInfo->lpfnHook=0;
	colorInfo->lpTemplateName=0;
	colorInfo->lStructSize=sizeof(CHOOSECOLOR);

	if (ChooseColor(colorInfo))
	{
		DWORD result=colorInfo->rgbResult;

		//Swap colours round
		DWORD colorRed=GetBValue(result);
		DWORD colorGreen=GetGValue(result);
		DWORD colorBlue=GetRValue(result);

		result=RGB(colorRed,colorGreen,colorBlue);

		delete colorInfo;
		return result;
	}
	else
	{
		delete colorInfo;
		return -1;
	}
}

EXPORT CGadgetFont* fontDialog()
{
	LPCHOOSEFONT fontInfo=new CHOOSEFONT;
	LOGFONT*	 fontOptions=new LOGFONT;

	ZeroMemory(fontInfo,sizeof(CHOOSEFONT));

	fontInfo->lStructSize=sizeof(CHOOSEFONT);
	fontInfo->Flags=CF_SCREENFONTS;
	fontInfo->lpLogFont=fontOptions;
	fontInfo->hwndOwner=dialogOwner();


	if (ChooseFont(fontInfo))
	{
		CGadgetFont* newFont=new CGadgetFont;

		newFont->create(fontOptions);		
		
		delete fontInfo;
		delete fontOptions;

		return newFont;
	}
	else
	{
		delete fontInfo;
		delete fontOptions;
		return 0;
	}
}

EXPORT DWORD folderDialog()
{
	//Fill out property information for folder dialog
	BROWSEINFOA browseInfo;
	browseInfo.hwndOwner=dialogOwner();
	browseInfo.iImage=0;
	browseInfo.lParam=0;
	browseInfo.lpfn=0;
	browseInfo.lpszTitle="Browse For Folder";
	browseInfo.pidlRoot=0;
	browseInfo.pszDisplayName=new char[MAX_PATH];
	browseInfo.ulFlags=0;
	

	//Create an item ID list 
	ITEMIDLIST* path=new ITEMIDLIST;

	//Call shell functions to display folder and convert the result to a string path
	path=SHBrowseForFolderA(&browseInfo);

	char* fullPath=new char[MAX_PATH];
	SHGetPathFromIDListA(path,fullPath);

	//Clean up and delete items

	//BUGBUG - Deleting path causes an assertion failure, but will failure to do this cause a memory leak?
	//delete path;
	delete browseInfo.pszDisplayName;

	//Return string
	DWORD outString=returnString(fullPath);
	return outString;
}
