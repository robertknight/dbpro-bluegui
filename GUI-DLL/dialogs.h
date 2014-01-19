#ifndef DIALOG_H
#define DIALOG_H

extern HWND dialogOwnerWindow;

EXPORT void			message(char* caption, char* messageText);
EXPORT void			messageNoCaption(char* messageText);
EXPORT DWORD		openDialog_noInitDir(DWORD oldString, char* caption, char* fileTypes, int options);
EXPORT DWORD		openDialog(DWORD oldString, char* caption, char* fileTypes, int options, char* initDir);
EXPORT DWORD		openDialog_noOptions(DWORD oldString, char* caption, char* fileTypes);
EXPORT DWORD		saveDialog_noInitDir(DWORD oldString, char* caption, char* fileTypes);
EXPORT DWORD		saveDialog(DWORD oldString, char* caption, char* fileTypes, char* initDir);
char*				fileDialog(OPENFILENAME& dialog, char* caption, char* fileTypes, int options=0, char* initDir=0);
EXPORT DWORD		colorDialog();

HWND				dialogOwner();

#endif