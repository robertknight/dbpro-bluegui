#include "GUI-DLL.h"



CGadgetMenu::CGadgetMenu()
{
	//Init vars
	myWindow=0;
	myParentMenu=0;
	myParentIdentifier=0;

}

void CGadgetMenu::create()
{
	myMenu=CreateMenu();

	MENUINFO info;

	info.cbSize=sizeof(MENUINFO);
	info.fMask=MIM_STYLE|MIM_MENUDATA|MIM_APPLYTOSUBMENUS;

	if (CApplication::isWindowsNT)
	{
		info.dwStyle=MNS_CHECKORBMP;
	}
	else
	{
		info.dwStyle=0;
	}

	info.dwMenuData=(DWORD)this;

	SetMenuInfo(myMenu,&info);
	
}

void CGadgetMenu::destroy()
{
	if (myWindow)
	{
		SetMenu(myWindow,0);
	}

	if (myParentMenu)
	{
		RemoveMenu(myParentMenu,myParentIdentifier,MF_BYCOMMAND);
		if (myWindow)
		{
			DrawMenuBar(myWindow);
		}
	}

	DestroyMenu(myMenu);
}

void CGadgetMenu::createPopup()
{
	myMenu=CreatePopupMenu();
}

void CGadgetMenu::attachToWindow(HWND window)
{
	SetMenu(window,myMenu);
	DrawMenuBar(window);
	myWindow=window;

	
	
}

void CGadgetMenu::attachToMenu(HMENU menu, int item)
{
	myParentMenu=menu;
	myParentIdentifier=item;
	
	MENUITEMINFO menuInfo;
	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_SUBMENU;
	menuInfo.hSubMenu=myMenu;

	SetMenuItemInfo(menu,(UINT)item,FALSE,&menuInfo);
}

void CGadgetMenu::addItem(char* text, int returnValue, int insertBefore, char* menuIcon)
{
	DWORD textLen	=strlen(text);
	char* tempText	=new char[textLen+1];

	//Copy string to temporary buffer
	strcpy(tempText,text);

	//Replace "\" character with tab character for shortcut keys
	char* tabPos=strchr(tempText,92);
	if (tabPos)
	{
		*(tabPos)=9;
	}

	//Prepare menu item structure which specifies the menu item settings
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_STRING|MIIM_ID;
	menuInfo.fType=MFT_STRING;
	menuInfo.wID=(UINT)returnValue;
	menuInfo.cch=textLen;
	menuInfo.dwTypeData=tempText;

	//Load icon if specified
	if ((menuIcon) && (CApplication::isWindowsNT))
	{
		HBITMAP icon=(HBITMAP)LoadImage(0,menuIcon,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_LOADTRANSPARENT);
		
		menuInfo.fMask |= MIIM_BITMAP;
		menuInfo.fType |= MFT_BITMAP;
		menuInfo.hbmpItem = icon;
	}

	InsertMenuItem(myMenu,insertBefore,TRUE,&menuInfo);

	//Redraw the menu bar to ensure that it is shown if it previously had 0 items.
	if (myWindow)
	{
		DrawMenuBar(myWindow);
	}

	//Delete temporary buffer
	delete [] tempText;
}

void CGadgetMenu::addSplitter()
{
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_TYPE;
	menuInfo.fType=MFT_SEPARATOR;

	InsertMenuItem(myMenu,-1,TRUE,&menuInfo);
}

int CGadgetMenu::getItemChecked(int item)
{
//Get existing state information
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_STATE;

	GetMenuItemInfo(myMenu,(UINT)item,FALSE,&menuInfo);

	if (menuInfo.fState & MFS_CHECKED) // is MFS_CHECKED flag in fState?
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CGadgetMenu::getItemEnabled(int item)
{
	//Get existing state information
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_STATE;

	GetMenuItemInfo(myMenu,(UINT)item,FALSE,&menuInfo);

	if (menuInfo.fState & MFS_DISABLED) // is MFS_ENABLED flag in fState?
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void CGadgetMenu::setItemChecked(int item, int boolChecked)
{
//Get existing state information
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_STATE;

	DWORD enabledState=0;
	DWORD checkedState=0;

	if (getItemEnabled(item))
	{
		enabledState=MFS_ENABLED;
	}
	else
	{
		enabledState=MFS_DISABLED;
	}

	if (boolChecked)
	{
		checkedState=MFS_CHECKED;
	}
	else
	{
		checkedState=MFS_UNCHECKED;
	}

	menuInfo.fState=enabledState|checkedState;
		
//Add or remove checked property
	SetMenuItemInfo(myMenu,(UINT)item,FALSE,&menuInfo);
}

void CGadgetMenu::setItemEnabled(int item, int boolEnabled)
{
//Get existing state information
	MENUITEMINFO menuInfo;

	menuInfo.cbSize=sizeof(MENUITEMINFO);
	menuInfo.fMask=MIIM_STATE;

	DWORD enabledState=0;
	DWORD checkedState=0;

	if (boolEnabled)
	{
		enabledState=MFS_ENABLED;
	}
	else
	{
		enabledState=MFS_DISABLED;
	}

	if (getItemChecked(item))
	{
		checkedState=MFS_CHECKED;
	}
	else
	{
		checkedState=MFS_UNCHECKED;
	}

	menuInfo.fState=enabledState|checkedState;

//Add or removed enabled property
	SetMenuItemInfo(myMenu,(UINT)item,FALSE,&menuInfo);

//Redraw menu bar if this is a main window
	if (myWindow)
	{
		DrawMenuBar(myWindow);
	}
}

void CGadgetMenu::getItemText(int item, char* outCaption, int maxLength)
{
	GetMenuString(myMenu,(UINT)item,outCaption,maxLength,MF_BYCOMMAND);
}

void CGadgetMenu::setItemText(int item, char* caption)
{
	ModifyMenu(myMenu,item,MF_BYCOMMAND|MF_STRING,item,caption);
}

int CGadgetMenu::showPopup(int x, int y)
{
#ifdef DBPRO_DLL
	HWND win=CApplication::core->hWnd;
#else
	HWND win=NULL;
#endif

	return TrackPopupMenu(myMenu,TPM_RETURNCMD,x,y,0,win,0);
}

int CGadgetMenu::getItemCount()
{
	return GetMenuItemCount(myMenu);
}

void CGadgetMenu::setItemRadioChecked(int item, int firstGroupItem, int lastGroupItem)
{
		CheckMenuRadioItem(myMenu,(UINT)firstGroupItem,(UINT)lastGroupItem,(UINT)item,MF_BYCOMMAND);
}



	
	


