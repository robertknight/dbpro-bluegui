//Gadget Functions
//
#include "GUI-DLL.h"

class CGadget;
class CGadgetButton;
class CApplication;

HMODULE	CApplication::webLib;
DWORD	CApplication::demoTimer;
bool	CApplication::initSuccess;
bool	CApplication::timeout;
//bool	CApplication::isWindowsNT;

DWORD	CGadget::defaultStyles;


//Forward declarations
EXPORT_MANGLED void ReceiveCoreDataPtr ( LPVOID pCore );

EXPORT void					setDialogOwner(CGadget* gadget);

//Gadget creation
CGadget*					createButton(int x, int y, int width, int height, char* caption);
EXPORT CGadget*				createPanel(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createFrame(int x, int y, int width, int height, char* caption,CGadget* parent);
EXPORT CGadget*				createEdit(int x, int y, int width, int height, int settings,CGadget* parent);
EXPORT CGadgetStatic*		createLabel(int x, int y, int width, int height, char* caption, CGadget* parent);
EXPORT CGadget*				createCanvas(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createCheckbox(int x, int y, int width, int height, char* caption, CGadget* parent);
EXPORT CGadget*				createRadiobutton(int x, int y, int width, int height, char* caption, CGadget* parent);
EXPORT CGadget*				createListbox(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createListview(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createCombobox(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createTabs(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createUpDown_noBuddy(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createUpDown_buddy(int x, int y, int width, int height, CGadget* buddy, int style, CGadget* parent);
EXPORT CGadget*				createScrollbar(int x, int y, int width, int height, int style, CGadget* parent);
EXPORT CGadget*				createTrackbar(int x, int y, int width, int height, int style, CGadget* parent);
EXPORT CGadget*				createTreeview(int x, int y, int width, int height, CGadget* parent);
EXPORT CGadget*				createToolbar(CGadget* parent);
EXPORT CGadget*				createStatusbar(CGadget* parent);
EXPORT CGadget*				createWindow(int x, int y, int width, int height, 
								 char* caption, int styles, int extraStyles, int createTopMost, CGadget* parent);
EXPORT CGadget*				createGadgetFromWindow(HWND existingWindow);
EXPORT CGadget*				copyGadget(CGadget* existingGadget);

//Image List
EXPORT CGadgetImageList*	createImageList(int width, int height);
EXPORT void					addImageListItem(CGadgetImageList* imageList, char* fileName);
EXPORT void					addImageListItem_FromMem(CGadgetImageList* imageList, int imageIndex);

//Menu
EXPORT CGadgetMenu*			createMenu(CGadget* parent);
EXPORT CGadgetMenu*			createPopupMenu();
EXPORT CGadgetMenu*			createSubMenu(CGadgetMenu* parent, int item);
EXPORT void					addMenuItem(CGadgetMenu* menu, char* caption, int returnValue);
EXPORT void					addMenuSplitter(CGadgetMenu* menu);
EXPORT void					setMenuItemChecked(CGadgetMenu* menu, int item, int boolChecked);
EXPORT void					setMenuItemEnabled(CGadgetMenu* menu, int item, int boolEnabled);
EXPORT int					getMenuItemChecked(CGadgetMenu* menu, int item);
EXPORT int					getMenuItemEnabled(CGadgetMenu* menu, int item);
EXPORT DWORD				getMenuItemText(DWORD oldString, CGadgetMenu* menu, int item);
EXPORT int					getMenuItemCount(CGadgetMenu* menu);
EXPORT int					showPopupMenu(CGadgetMenu* menu);
EXPORT void					setMenuItemRadio(CGadgetMenu* menu, int firstGroupItem, int lastGroupItem, int selectItem);
   
//Events
EXPORT void					getEvent_wait(int waitEvent);
EXPORT void					getEvent_noWait();
EXPORT void					getEventStack();
EXPORT DWORD				eventSource();
EXPORT DWORD				eventType();
EXPORT DWORD				eventData();
EXPORT DWORD				eventDataEx();
EXPORT CGadget*				gadgetClicked();

//General Gadget Functions
EXPORT void					moveGadget(CGadget* gadget, int x, int y);
EXPORT void					resizeGadget(CGadget* gadget, int width, int height);
EXPORT void					setGadget(CGadget* gadget, int x, int y,int width, int height);
EXPORT void					paintGadget(CGadget* gadget);
EXPORT void					doGadgetAlign(CGadget* gadget);
EXPORT void					setGadgetAlign(CGadget* gadget, int align);
EXPORT void					setVisible(CGadget* gadget, int flag);
EXPORT void					bringToFront(CGadget* gadget);
EXPORT void					setGadgetEnabled(CGadget* gadget, DWORD flag);
EXPORT void					setGadgetParent(CGadget* gadget, CGadget* parent);


EXPORT CGadget*				getParentGadget(CGadget* gadget);


EXPORT void					deleteGadget(CGadget* gadget);
EXPORT int					deleteGadget_returnNull(CGadget* gadget);

EXPORT int					gadgetX(CGadget* gadget);
EXPORT int					gadgetY(CGadget* gadget);
EXPORT int					gadgetX_relative(CGadget* gadget, CGadget* relativeTo);
EXPORT int					gadgetY_relative(CGadget* gadget, CGadget* relativeTo);
EXPORT int					gadgetWidth(CGadget* gadget);
EXPORT int					gadgetHeight(CGadget* gadget);
EXPORT void					setGadgetText(CGadget* gadget,char* text);
EXPORT DWORD				getGadgetText(DWORD oldString, CGadget* gadget);
EXPORT int					getVisible(CGadget* gadget);
EXPORT HWND					gadgetHandle(CGadget* gadget);
EXPORT DWORD				getGadgetEnabled(CGadget* gadget);

//Helper Functions
EXPORT CGadget*				gadgetUnderMouse();	
EXPORT HWND					mainWindow();
EXPORT HWND					windowHandle(CGadget* gadget);
EXPORT CGadget*				activeGadget();
EXPORT int					mousePosX(CGadget* gadget);
EXPORT int					mousePosY(CGadget* gadget);
EXPORT int					mousePosX_noGadget();
EXPORT int					mousePosY_noGadget();
POINT						mousePos(CGadget* gadget);
EXPORT void					createHotKey(int hotKeyId, int requireAlt, int requireShift, int requireCtrl, int keyCode);
EXPORT void					deleteHotKey(int hotKeyId);
EXPORT int					getTextWidth(CGadgetFont* font, char* text);
EXPORT int					getTextHeight(CGadgetFont* font, char* text);

SIZE						getTextDimensions(CGadgetFont* font, char* text);

//Special Functions

//Panel Gadgets
	EXPORT void		setPanelBorderStyle(CGadgetStatic* gadget,int borderStyle);
	EXPORT void		setPanelImage(CGadgetStatic* gadget, char* imageFileName);
	EXPORT void		setPanelImageFromMem(CGadgetStatic* gadget, int imageIndex);

//Button Gadgets
	EXPORT void		setButtonImage(CGadgetButton* gadget, char* imageFileName);
	EXPORT void		setButtonImageFromMem(CGadgetButton* gadget, int imageIndex);

//Edit Gadgets
	EXPORT void		setReadOnly(CGadgetEdit* gadget, bool readOnly);
	EXPORT void		setSelStart(CGadgetEdit* gadget, int selStart);
	EXPORT void		setSelLen(CGadgetEdit* gadget, int selLength);

	EXPORT int		getReadOnly(CGadgetEdit* gadget);
	EXPORT int		getSelStart(CGadgetEdit* gadget);
	EXPORT int		getSelLen(CGadgetEdit* gadget);

//Checkbox Gadgets
	EXPORT void		setChecked(CGadget* gadget, bool checked);
	EXPORT int		getChecked(CGadgetCheckbox* gadget);

//Listbox/Combobox Gadgets
	EXPORT void		addItem(CGadget* gadget, char* caption);
	EXPORT void		addItem_index(CGadget* gadget, char* caption, int index);
	EXPORT void		removeItem(CGadget* gadget, int index);
	EXPORT void		changeItem(CGadget* gadget, int index, char* newCaption);
	EXPORT void		selectItem(CGadget* gadget, int index);
	EXPORT int		selectedItem(CGadget* gadget);
	EXPORT DWORD	itemText(DWORD oldString, CGadget* gadget, int index);
	EXPORT DWORD	itemCount(CGadget* gadget);

//Tab Gadgets
	EXPORT void		addTab(CGadgetTabs* gadget, char* caption);
	EXPORT void		addTab_index(CGadgetTabs* gadget, char* caption, int index);
	EXPORT void		removeTab(CGadgetTabs* gadget, int index);
	EXPORT void		changeTab(CGadgetTabs* gadget, int index, char* newCaption);
	EXPORT int		selectedTab(CGadgetTabs* gadget);
	EXPORT DWORD	tabCaption(DWORD oldString, CGadgetTabs* gadget, int index);
	EXPORT int		tabCount(CGadgetTabs* gadget);

//Statusbar Gadgets
	EXPORT void		addStatusPanel(CGadgetStatusbar* gadget, int width);
	EXPORT void		setStatusText(CGadgetStatusbar* gadget, int panelIndex, char* caption);
	EXPORT void		removeStatusPanel(CGadgetStatusbar* gadget, int index);
	EXPORT int		statusPanelCount(CGadgetStatusbar* gadget);
	EXPORT DWORD	getStatusText(DWORD oldString, CGadgetStatusbar* gadget, int index);

//Scrollbar Gadgets
	EXPORT void		setScrollbarRange(CGadgetScrollbar* gadget, int min, int max);
	EXPORT void		setScrollbarPosition(CGadgetScrollbar* gadget, int position);
	EXPORT int		getScrollbarPosition(CGadgetScrollbar* gadget);
	EXPORT int		getScrollbarMin(CGadgetScrollbar* gadget);
	EXPORT int		getScrollbarMax(CGadgetScrollbar* gadget);
	EXPORT void		setScrollbarThumbSize(CGadgetScrollbar* gadget, int size);
	EXPORT int		getScrollbarThumbSize(CGadgetScrollbar* gadget);

//UpDown Gadgets
	EXPORT void		setUpDownRange(CGadgetUpDown* gadget, int min, int max);
	EXPORT void		setUpDownPosition(CGadgetUpDown* gadget, int position);

	EXPORT int		getUpDownPosition(CGadgetUpDown* gadget);
	EXPORT int		getUpDownMin(CGadgetUpDown* gadget);
	EXPORT int		getUpDownMax(CGadgetUpDown* gadget);

//Toolbar Gadgets
	EXPORT void		addToolbarButton_image(CGadgetToolbar* gadget, char* fileName, int makeCheckButton);
	EXPORT void		addToolbarButton_index(CGadgetToolbar* gadget, int index, int makeCheckButton);
	EXPORT void		addStandardToolbarButtons(CGadgetToolbar* gadget);
	EXPORT void		removeToolbarButton(CGadgetToolbar* gadget, int index); //Not done
	EXPORT void		addToolbarSplitter(CGadgetToolbar* gadget);

	EXPORT int		getToolbarItem(CGadgetToolbar* gadget);

//Trackbar Gadgets
	EXPORT void		setTrackbarRange(CGadgetTrackbar* gadget, int min, int max);
	EXPORT void		setTrackbarPosition(CGadgetTrackbar* gadget, int position);
	EXPORT void		setTrackbarTickFrequency(CGadgetTrackbar* gadget, int frequency);
	EXPORT int		getTrackbarMin(CGadgetTrackbar* gadget);
	EXPORT int		getTrackbarMax(CGadgetTrackbar* gadget);
	EXPORT int		getTrackbarPosition(CGadgetTrackbar* gadget);

//Canvas Gadgets
	EXPORT void		drawText(CGadgetCanvas* gadget, int x, int y, char* text);
	EXPORT void		drawLine(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2);
	EXPORT void		drawRect(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2);
	EXPORT void		drawEllipse(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2);
	EXPORT void		drawPoint(CGadgetCanvas* gadget, int x, int y, DWORD color);

//Listview gadgets
	EXPORT void		addListViewItem(CGadgetListview* gadget, char* caption, int image);
	EXPORT void		setListViewImageList(CGadgetListview* gadget, CGadgetImageList* images);
	EXPORT void		setListViewImageList_withIndex(CGadgetListview* gadget, CGadgetImageList* images, int listIndex);
	EXPORT void		addListViewColumn(CGadgetListview* gadget, char* caption, int width);
	EXPORT int		getListViewItemCount(CGadgetListview* gadget);
	EXPORT void		setListViewStyle(CGadgetListview* gadget, int style);
	EXPORT void		setListViewItemText(CGadgetListview* gadget, int index, int subItemIndex, char* caption);
	EXPORT void		selectListViewItem(CGadgetListview* gadget, int index);
	EXPORT int		selectedListViewItem(CGadgetListview* gadget);
	EXPORT void		removeListViewItem(CGadgetListview* gadget, int index);

//Treeview gadgets
	EXPORT HTREEITEM	addTreeViewItem(CGadgetTreeview* gadget, HTREEITEM parent, int image, char* caption);
	EXPORT HTREEITEM	addTreeViewItem_index(CGadgetTreeview* gadget, HTREEITEM parent, int image, char* caption, HTREEITEM insertAfter);
	EXPORT void			setTreeViewImageList(CGadgetTreeview* gadget, CGadgetImageList* imageList, int index);
	EXPORT void			removeTreeViewItem(CGadgetTreeview* gadget, HTREEITEM item);
	EXPORT HTREEITEM	selectedTreeViewItem(CGadgetTreeview* gadget);
	EXPORT void			selectTreeViewItem(CGadgetTreeview* gadget, HTREEITEM item);
	EXPORT void			setTreeViewItemText(CGadgetTreeview* gadget, HTREEITEM item, char* text);

BOOL WINAPI DllMain(HINSTANCE hinstDLL,  DWORD fdwReason, LPVOID lpvReserved)
{
	CApplication::hInstance=hinstDLL;

	return TRUE;
}

// Get Core Data Pointer here - called by DBPro after loading DLL
//Use mangled export so that DBPro can find the DLL function

//DBPRO Functions

EXPORT void setupBlueLibrary()
{
	GlobStruct* newCore=new GlobStruct;
	ZeroMemory(newCore,sizeof(newCore));

	ReceiveCoreDataPtr(newCore);
}

EXPORT void processEvents(HWND window)
{
	MSG msg;

	while(GetMessage(&msg,window,0,0))
	{ 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
}

EXPORT void setMainWindow(HWND mainWindow, HINSTANCE appInstance)
{
	CApplication::core->hWnd=mainWindow;
	CApplication::core->hInstance=appInstance;

	//Add WS_CLIPCHILDREN property to DBPro window so that it does not cover over gadgets
	SetWindowLong(CApplication::core->hWnd,GWL_STYLE,GetWindowLong(CApplication::core->hWnd,GWL_STYLE)|WS_CLIPCHILDREN);
	

	//Subclass main window
	CApplication::originalMainWindowProc=GetWindowLong(CApplication::core->hWnd,GWL_WNDPROC);
		
	SetWindowLong(CApplication::core->hWnd,GWL_WNDPROC,(LONG)SubClassFunc);
}

EXPORT_MANGLED void ReceiveCoreDataPtr ( LPVOID pCore )
{
	core = (GlobStruct*)pCore;

#ifdef DEMO_VERSION
	MessageBox(0,"This program was created using the trial version of BlueGUI v2 (http://blue.robert-knight.net)","Trial Version",0);
	
#endif
	CApplication::timeout = false;

	//Enable common controls
	GSetupCommonControls();

	//Load library for richedit controls
	LoadLibrary("riched20.dll");

	//Setup COM for Browse Folder dialog
	CoInitialize(0);

	//Load library for AlphaBlend function
	HMODULE imageLib=LoadLibrary("msimg32.dll");
	CApplication::alphaBlend = (PFAlphaBlend) GetProcAddress(imageLib,"AlphaBlend");

	
	CApplication::core = core;


	CApplication::initSuccess=false;
	CApplication::paintWindow=CApplication::core->hWnd;

	//Set dialog parent
	setDialogOwner(0);

	//OS Specific Settings
	OSVERSIONINFO osInfo;
	osInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osInfo);
	
	CApplication::isWindowsNT=(osInfo.dwPlatformId==VER_PLATFORM_WIN32_NT);

	CGadgetButton::BoldFocusButton=0;

	//core->ProcessMessageFunction=0;

	//Get DBPro function pointers

	//Check that memblocks DLL is loaded & image DLL is loaded
	if ((CApplication::core->g_Memblocks) && (CApplication::core->g_Image))
	{		
		CApplication::makeMemblockFromImage	=(PFMakeMemblockFromImage)	GetProcAddress(CApplication::core->g_Memblocks,"?CreateMemblockFromImage@@YAXHH@Z");
		CApplication::memblockExists		=(PFMemblockExists)			GetProcAddress(CApplication::core->g_Memblocks,"?MemblockExist@@YAHH@Z");
		CApplication::getMemblockPtr		=(PFGetMemblockPtr)			GetProcAddress(CApplication::core->g_Memblocks,"?GetMemblockPtr@@YAKH@Z");
		CApplication::getMemblockSize		=(PFGetMemblockSize)		GetProcAddress(CApplication::core->g_Memblocks,"?GetMemblockSize@@YAHH@Z");
		CApplication::deleteMemblock		=(PFDeleteMemblock)			GetProcAddress(CApplication::core->g_Memblocks,"?DeleteMemblock@@YAXH@Z");
		CApplication::imageExists			=(PFImageExists)			GetProcAddress(CApplication::core->g_Image,"?GetExistEx@@YAHH@Z");
		CApplication::makeMemblock			=(PFMakeMemblock)			GetProcAddress(CApplication::core->g_Memblocks,"?MakeMemblock@@YAXHH@Z");
		CApplication::makeImageFromMemblock =(PFMakeImageFromMemblock)	GetProcAddress(CApplication::core->g_Memblocks,"?CreateImageFromMemblock@@YAXHH@Z");
	}

	//Attempt to load the web browser library
	CApplication::webLib=LoadLibrary("gui2_web.dll");

	if (CApplication::webLib)
	{
		registerCGadgetWebBrowser();
	}

	//Perform application init stuff
	CGadget::events = new CGadgetEvents;
	CGadget::defaultStyles = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_TABSTOP;

	

	//Add WS_CLIPCHILDREN property to DBPro window so that it does not cover over gadgets
	
	SetWindowLong(CApplication::core->hWnd,GWL_STYLE,GetWindowLong(CApplication::core->hWnd,GWL_STYLE)|WS_CLIPCHILDREN);
	

	//Subclass main window
	CApplication::originalMainWindowProc=GetWindowLong(CApplication::core->hWnd,GWL_WNDPROC);
		
	SetWindowLong(CApplication::core->hWnd,GWL_WNDPROC,(LONG)SubClassFunc);

	//Load cursors
	CApplication::sizingCursor = LoadCursor(0,IDC_SIZEWE);

	//Register custom gadget types
	registerCGadgetWindow(); //CGadgetWindow type


#ifdef DEMO_VERSION
	CApplication::demoTimer = GetTickCount();
	
#endif	
	
}



//String handling
	DWORD returnString(LPSTR pStr)
	{
		if (CApplication::core->CreateDeleteString)
		{
			LPSTR pReturnString=NULL;
			DWORD size=strlen(pStr);
		
			CApplication::core->CreateDeleteString((DWORD*)&pReturnString,size+1);

			strcpy(pReturnString,pStr);

			delete [] pStr;

			return (DWORD)(pReturnString);
		}
		else
		{
			//Not a DBPro DLL, just return the passed string
			return (DWORD)pStr;
		}
	}

	//Deletes an existing string if valid
	void deleteString(DWORD pOldString)
	{
		if (pOldString)
		{
			CApplication::core->CreateDeleteString((DWORD*)&pOldString,0);
		}
	}
  

EXPORT void startBlue(char* user, char* compare)
{
	if ((strcmp(user,"RK")==0) || (strcmp(user,"DEMO")==0))
	{
		CApplication::initSuccess=false;

		terminateApp();

		return;
	}

	if ( (strlen(user)==0) && (strlen(compare)==0) )
		terminateApp();

	//Encrypt the user string and we should end up with the compare string
	//The compare string is 
	Crypt(user,strlen(user),"B2ENCSTR",strlen("B2ENCSTR"));

	int len=strlen(user);
	int j=0;
	int result=0;

	char buffer[3];

	//Check string is not null
	while (!len)
		terminateApp();

	for (int i=0;i<len;i++)
	{
		//Compare each character, 
		for (int k=0;k<2;k++)
		{
			buffer[k]=*(compare+j);
			j++;
		}
		buffer[2]=0;

		sscanf((char*)&buffer,"%x",&result);

		//debugInt("Result",result);

		if (result!=*(user+i))
		{
			terminateApp();
		}
	}

	//Check string is not null
	while (!len)
		terminateApp();

	CApplication::initSuccess=true;
}

EXPORT void createHotKey(int hotKeyId, int requireAlt, int requireShift, int requireCtrl, int keyCode)
{
	SHORT virtualKey=VkKeyScan((char)keyCode);

	char virtualKeyCode=LOBYTE(virtualKey);
	
	UINT keyModifiers=0;

	if (requireAlt)		{ keyModifiers=keyModifiers|MOD_ALT;	}
	if (requireShift)	{ keyModifiers=keyModifiers|MOD_SHIFT;	}
	if (requireCtrl)	{ keyModifiers=keyModifiers|MOD_CONTROL;}

#ifdef DBPRO_DLL
	RegisterHotKey(CApplication::core->hWnd,hotKeyId,keyModifiers,(UINT)virtualKeyCode);
#endif
}

EXPORT DWORD getSystemColor(int index)
{
	//COLOR_3DLIGHT
	return convertColor(GetSysColor(index));
}

EXPORT void resetMainWindow()
{
	SetWindowLong(CApplication::core->hWnd,GWL_STYLE,GetWindowLong(CApplication::core->hWnd,GWL_STYLE)|WS_CLIPCHILDREN);
}

EXPORT void deleteHotKey(int hotKeyId)
{
	UnregisterHotKey(CApplication::core->hWnd, hotKeyId);
}

EXPORT int mousePosX(CGadget* gadget)
{
	POINT mousePosInfo=mousePos(gadget);

	return mousePosInfo.x;
}

EXPORT int mousePosY(CGadget* gadget)
{
	POINT mousePosInfo=mousePos(gadget);

	return mousePosInfo.y;
}

EXPORT int mousePosX_noGadget()
{
	POINT mousePosInfo=mousePos(0);

	return mousePosInfo.x;
}

EXPORT int mousePosY_noGadget()
{
	POINT mousePosInfo=mousePos(0);

	return mousePosInfo.y;
}

//Returns mouse position relative to client area of specified gadget
POINT mousePos(CGadget* gadget)
{
	POINT position;

	GetCursorPos(&position);

	WINDOWINFO windowInfo;
	ZeroMemory(&windowInfo,sizeof(windowInfo));
	windowInfo.cbSize=sizeof(WINDOWINFO);

	if (gadget)
	{
		GetWindowInfo(gadget->getHwnd(),&windowInfo);
	}
	else
	{

		GetWindowInfo(CApplication::core->hWnd,&windowInfo);

	}

	int mouseY=position.y-windowInfo.rcClient.top;
	int mouseX=position.x-windowInfo.rcClient.left;

	//Return -1 if the mouse is outside of the specified window
	if ((mouseY < 0)													|| 
		(mouseY > (windowInfo.rcClient.bottom-windowInfo.rcClient.top)) ||
		(mouseX < 0)													||
		(mouseX > (windowInfo.rcClient.right-windowInfo.rcClient.left)))
	{
		POINT out;
		out.x=-1;
		out.y=-1;

		return out;
	}
	else
	{
		POINT out;
		out.x=mouseX;
		out.y=mouseY;

		return out;
	}
}
  
//Destroy gadget
EXPORT void deleteGadget(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->destroy();
	delete gadget;
}

//Destroy gadget and return 0
EXPORT int deleteGadget_returnNull(CGadget* gadget)
{
	MReturnIfNull(gadget)

	gadget->destroy();
	delete gadget;

	return 0;
}

//Gadget Creation Functions
EXPORT CGadget* createButton(int x, int y, int width, int height, char* caption,bool flat,CGadget* parent)
{
	//Security
	terminateApp();
	
	CGadgetButton*  newGadget=new CGadgetButton;
	HWND			parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}
	
	//Create our button
	if (newGadget->create(x,y,width,height,caption,flat,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createPanel(int x, int y, int width, int height, CGadget* parent)
{
	terminateApp();

	CGadgetStatic*	newGadget=new CGadgetStatic;
	HWND			parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create our panel
	if (newGadget->create(x,y,width,height,0,parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		//newGadget->setTransparent(true);
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createFrame(int x, int y, int width, int height, char* caption,CGadget* parent)
{
	terminateApp();

	CGadgetFrame*		newGadget=new CGadgetFrame;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create our panel
	if (newGadget->create(x,y,width,height,caption, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createEdit(int x, int y, int width, int height, int settings,CGadget* parent)
{
	terminateApp();

	CGadgetEdit* newGadget=new CGadgetEdit;
	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create edit
	if (newGadget->create(x, y, width, height, settings,parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();

		//newGadget->setColors(RGB(255,0,0),RGB(0,0,0));

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createRichEdit(int x, int y, int width, int height,CGadget* parent)
{
	terminateApp();

	CGadgetRichEdit* newGadget=new CGadgetRichEdit;
	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create edit
	if (newGadget->create(x, y, width, height, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();

		//newGadget->setColors(RGB(255,0,0),RGB(0,0,0));

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadgetStatic* createLabel(int x, int y, int width, int height, char* caption,CGadget* parent)
{
	terminateApp();

	CGadgetStatic* newGadget=new CGadgetStatic;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create label
	if (newGadget->create(x, y, width, height, caption,parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		//newGadget->setTransparent(true);
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

/*
	COMMAND_HELP

	handle gadget=createTransparentLabel(int x, int y, int width, int height, str caption, handle parent)

	Creates a label gadget with a transparent background.  The parameters and return value are the same as 
	the <a href="createLabel.html">createLabel</a> command.

	x: Horizontal position of the new gadget (pixels across from left-edge of parent)
	y: Vertical position of the new gadget (pixels down from top-edge of parent)
	width: Width of new gadget in pixels
	height: Height of new gadget in pixels
	caption: Caption of new label
	parent: Set to the handle of an existing gadget to create the new label as a child of that gadget, or 0 \
	to place the new gadget inside the main DarkBASIC application window.

*/
EXPORT CGadgetStatic* createTransparentLabel(int x, int y, int width, int height, char* caption, CGadget* parent)
{
	CGadgetStatic* gadget=createLabel(x,y,width,height,caption,parent);

	if (gadget)
		gadget->setTransparent(true);

	return gadget;
}

EXPORT CGadget* createCheckbox(int x, int y, int width, int height, char* caption, CGadget* parent)
{
	terminateApp();

	CGadgetCheckbox* newGadget=new CGadgetCheckbox;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create Checkbox
	if (newGadget->create(x, y, width, height, caption,parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createRadiobutton(int x, int y, int width, int height, char* caption, CGadget* parent)
{
	terminateApp();

	CGadgetRadiobutton* newGadget=new CGadgetRadiobutton;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create Checkbox
	if (newGadget->create(x, y, width, height, caption,parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createListbox(int x, int y, int width, int height, CGadget* parent)
{
	terminateApp();

	CGadgetListbox* newGadget=new CGadgetListbox;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create listbox
	if (newGadget->create(x, y, width, height, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createCombobox(int x, int y, int width, int height, CGadget* parent)
{
	terminateApp();

	CGadgetCombobox* newGadget=new CGadgetCombobox;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create combobox
	if (newGadget->create(x, y, width, height, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}



EXPORT CGadget* createTabs_style(int x, int y, int width, int height, int style, CGadget* parent)
{
	terminateApp();

	CGadgetTabs* newGadget=new CGadgetTabs;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create combobox
	if (newGadget->create(x, y, width, height, style, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createTabs_noStyle(int x, int y, int width, int height, CGadget* parent)
{
	return createTabs_style(x,y,width,height,0,parent);
}

EXPORT CGadget* createScrollbar(int x, int y, int width, int height, int style, CGadget* parent)
{
	terminateApp();

	CGadgetScrollbar* newGadget=new CGadgetScrollbar;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create scrollbar
	if (newGadget->create(x, y, width, height, style, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createProgressBar(int x, int y, int width, int height, CGadget* parent)
{
	terminateApp();

	CGadgetProgressBar* newGadget=new CGadgetProgressBar;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create progress bar
	if (newGadget->create(x, y, width, height, parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}


EXPORT CGadget* createStatusbar(CGadget* parent)
{
	terminateApp();

	CGadgetStatusbar* newGadget=new CGadgetStatusbar;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create statusbar
	if (newGadget->create(parentWindow))
	{
		//Apply default font
		newGadget->setDefaultFont();
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createUpDown_buddy_vert(int x, int y, int width, int height, CGadget* buddy, CGadget* parent)
{
	return createUpDown_buddy(x,y,width,height,buddy,0,parent);
}

EXPORT CGadget* createUpDown_noBuddy_vert(int x, int y, int width, int height, CGadget* parent)
{
	return createUpDown_buddy(x, y, width, height, 0, 0, parent);
}

EXPORT CGadget* createUpDown_buddy(int x, int y, int width, int height, CGadget* buddy, int style, CGadget* parent)
{
	terminateApp();

	CGadgetUpDown*  newGadget=new CGadgetUpDown;
	HWND			parentWindow=0;

	//The buddy window must not be the parent window
	if ((buddy) && (parent) && (buddy == parent))
	{
		delete newGadget;
		return 0;
	}

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}
	
	//Create our button
	if (newGadget->create(x,y,width,height,style,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		//Set the new buddy window
		if (buddy)
		{
			newGadget->setBuddy(buddy->getHwnd());
			
			if ((width) && (height))
			{
				newGadget->move(newGadget->getX(),newGadget->getY(),width,height);
			}
		}

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createToolbar(CGadget* parent)
{
	terminateApp();

	CGadgetToolbar* newGadget=new CGadgetToolbar;
	//CGadget* ng=newGadget;

	HWND			parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create toolbar
	if (newGadget->create(parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();
	
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

/*
EXPORT CGadget* createToolWindow(int x, int y, int width, int height, char* caption, CGadget* parent)
{
	return createWindow(x,y,width,height,caption,
}
*/

EXPORT CGadget* createWindow(int x, int y, int width, int height, char* caption, int styles, int extraStyles, int createTopMost, CGadget* parent)
{
	terminateApp();

	CGadgetWindow*	newGadget=new CGadgetWindow;
	HWND			parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		if (!createTopMost)
		{
			parentWindow=core->hWnd;
		}
	}

	//Create window
	if (newGadget->create(x,y,width,height,caption,styles,extraStyles,createTopMost,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createTrackbar(int x, int y, int width, int height, int style, CGadget* parent)
{
	terminateApp();

	CGadgetTrackbar*	newGadget=new CGadgetTrackbar;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create window
	if (newGadget->create(x,y,width,height,style,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createCanvas(int x, int y, int width, int height, CGadget* parent)
{
	terminateApp();

	CGadgetCanvas*	newGadget=new CGadgetCanvas;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create window
	if (newGadget->create(x,y,width,height,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createWebBrowser(int x, int y, int width, int height, CGadget* parent)
{
	CGadgetWebBrowser*	newGadget=new CGadgetWebBrowser;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create window
	if (newGadget->create(x,y,width,height,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createListview(int x, int y, int width, int height, CGadget* parent)
{
	CGadgetListview*	newGadget=new CGadgetListview;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create window
	if (newGadget->create(x,y,width,height,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createTreeview(int x, int y, int width, int height, CGadget* parent)
{
	CGadgetTreeview*	newGadget=new CGadgetTreeview;
	HWND				parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
		parentWindow=core->hWnd;
	}

	//Create window
	if (newGadget->create(x,y,width,height,parentWindow))
	{
		//Apply a default font
		newGadget->setDefaultFont();

		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}

EXPORT CGadget* createGadgetFromWindow(HWND existingWindow)
{
	CGadget* newGadget = new CGadget;

	if (newGadget->createFromWindow(existingWindow))
	{
		return newGadget;
	}
	else
	{
		delete newGadget;
		return 0;
	}
}


EXPORT CGadget* copyGadget(CGadget* existingGadget)
{
	if (existingGadget)
	{
		//Create new gadget with same properties
		char*	className=new char[100];
		char*	windowText=0;
		DWORD	windowStyle;

		//Get property info
		windowText=existingGadget->getText();
		GetClassName(existingGadget->getHwnd(),className,100);
		windowStyle=GetWindowLong(existingGadget->getHwnd(),GWL_STYLE);

		CGadget* newGadget = new CGadget;

		HWND newWindow=CreateWindow(className,windowText,windowStyle,existingGadget->getX(),existingGadget->getY()
			,existingGadget->getWidth(),existingGadget->getHeight(),GetParent(existingGadget->getHwnd()),0,0,0);

		if (newWindow)
		{
			newGadget->createFromWindow(newWindow);
			newGadget->setDefaultFont();
			return newGadget;
		}
		else
		{
			delete newGadget;
			return 0;
		}

		delete windowText;
		delete className;
	}
	else
	{
		return 0;
	}
}

//Returns the window handle of the main window
EXPORT HWND mainWindow()
{
	return core->hWnd;
	
}

EXPORT HWND desktopWindow()
{
	return GetDesktopWindow();
}

//Events tracking functions
/*
EXPORT void getEvent_noWait()
{
	CGadget::getEvents(false);
}

EXPORT void getEvent_wait(int waitEvent)
{
	if (waitEvent)
	{
		CGadget::getEvents(true);
	}
	else
	{
		CGadget::getEvents(false);
	}
}
*/

EXPORT void postEvent(DWORD evSource, DWORD evType, DWORD evData, DWORD evDataEx)
{
	CGadget::events->push(evSource,evType,evData,evDataEx);
}

EXPORT void flushEvents()
{
	CGadget::events->flush();
}

EXPORT void getEvent_noWait()
{
	CGadget::events->pop();
}

EXPORT void getEvent_wait(int waitEvent)
{
	if (!waitEvent) 
	{
		getEvent_noWait();
		return;
	}

	do
	{
		CGadget::events->pop();
	} while (eventSource()==0);
}

EXPORT DWORD eventSource()
{
	return CGadget::events->getEventSource();
}

EXPORT DWORD eventType()
{
	return CGadget::events->getEventType();
}

EXPORT DWORD eventData()
{
	return CGadget::events->getEventData();
}

EXPORT DWORD eventDataEx()
{
	return CGadget::events->getEventDataEx();
}

EXPORT void clearEvent()
{
	CGadget::events->clearEvent();
}

EXPORT CGadget* gadgetClicked()
{
	CGadget* lastClicked=CGadget::lastClicked;
	CGadget::lastClicked=0;
	return lastClicked;
}


//General Gadget Functions
EXPORT void moveGadget(CGadget* gadget,int x, int y)
{
	MReturnVoidIfNull(gadget)

	gadget->move(x,y,gadget->getWidth(),gadget->getHeight());
	
}

EXPORT void resizeGadget(CGadget* gadget,int width, int height)
{
	MReturnVoidIfNull(gadget)

	gadget->move(gadget->getX(),gadget->getY(),width,height);
}

EXPORT void resizeGadget_client(CGadget* gadget, int width, int height, int clientSize)
{
	MReturnVoidIfNull(gadget)

	if (clientSize)
	{
		gadget->move(gadget->getX(),gadget->getY(),width,height,true);
	}
	else
	{
		gadget->move(gadget->getX(),gadget->getY(),width,height,false);
	}
}

EXPORT void setGadget(CGadget* gadget,int x,int y, int width, int height)
{
	MReturnVoidIfNull(gadget)

	gadget->move(x,y,width,height);
}

EXPORT void paintGadget(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->update();
}

/*  
	COMMAND_HELP

	setMaxTextLength handle gadget, int length

	Sets the maximum length of text a user can enter into an edit or richedit gadget.
	This does not affect the length of text that can be added to the control using setGadgetText.

	gadget:  Handle to an edit or richedit gadget
	length:  The length of text which the user can enter.
*/
EXPORT void setMaxTextLength(CGadgetEdit* gadget, int length)
{
	MReturnVoidIfNull(gadget)

	//Code works for richedits as well
	gadget->setMaxTextLength(length);
}

EXPORT void setGadgetText(CGadget* gadget,char* text)
{
	MReturnVoidIfNull(gadget)

	gadget->setText(text);
}

EXPORT DWORD getGadgetText(DWORD oldString, CGadget* gadget)
{
	MReturnIfNull(gadget)

	deleteString(oldString);
	
	//Allocate space for gadget text
	char* gadgetText=gadget->getText();
	DWORD outString=returnString(gadgetText); //Note returnString deletes the memory for the string passed in
	

	return outString;
}

EXPORT void setGadgetParent(CGadget* gadget, CGadget* parent)
{
	MReturnVoidIfNull(gadget)
	MReturnVoidIfNull(parent)

	gadget->setParent(parent->getHwnd());
}

EXPORT int gadgetX(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getX(false);
}

EXPORT int gadgetY(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getY(false);
}

EXPORT int gadgetX_relative(CGadget* gadget, CGadget* relativeTo)
{
	MReturnIfNull(gadget)
	MReturnIfNull(relativeTo)

	return (gadget->getX(true))-(relativeTo->getX(true,true));
}

EXPORT int gadgetY_relative(CGadget* gadget, CGadget* relativeTo)
{
	MReturnIfNull(gadget)
	MReturnIfNull(relativeTo)

	return gadget->getY(true)-relativeTo->getY(true,true);
}

EXPORT int gadgetWidth(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getWidth();
}

EXPORT int gadgetHeight(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getHeight();
}

EXPORT int gadgetClientWidth(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getClientWidth();
}

EXPORT int gadgetClientHeight(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getClientHeight();
}

EXPORT void setGadgetData(CGadget* gadget, DWORD data)
{
	MReturnVoidIfNull(gadget)

	gadget->setData(data);
}

EXPORT DWORD getGadgetData(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getData();
}


EXPORT void setVisible(CGadget* gadget, int visible)
{
	MReturnVoidIfNull(gadget)

	gadget->setVisible((DWORD)visible);
}

EXPORT void setVisible_centerFlag(CGadget* gadget, int visible, BOOL centerInParent)
{
	MReturnVoidIfNull(gadget)

	gadget->setVisible((DWORD)visible,centerInParent);
}

EXPORT int getVisible(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getVisible();
}

EXPORT void bringToFront(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->bringToFront();
}

EXPORT void sendToBack(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->sendToBack();
}

EXPORT void setGadgetEnabled(CGadget* gadget, DWORD flag)
{
	MReturnVoidIfNull(gadget)

	gadget->setEnabled(flag);
}

EXPORT DWORD getGadgetEnabled(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getEnabled();
}

EXPORT void setGadgetTimer(CGadget* gadget, DWORD milliseconds)
{
	MReturnVoidIfNull(gadget)
	
	gadget->setTimer(milliseconds);
}

EXPORT CGadget* activeGadget()
{
/*
	DWORD			threadId=0;
	GUITHREADINFO	guiInfo;

	guiInfo.cbSize=sizeof(GUITHREADINFO);
	guiInfo.hwndFocus=0;

	threadId=GetWindowThreadProcessId(core->hWnd,0);

	//Use this method instead of GetActiveWindow() / GetForegroundWindow() because they
	//only appear to return top-level windows
	GetGUIThreadInfo(threadId,&guiInfo);
*/
	//HWND activeWindow=guiInfo.hwndFocus;

	HWND activeWindow=GetFocus();

	if (activeWindow)
	{
		//Pointer to the gadget is stored in the window's user data
		CGadget* gadget=(CGadget*)GetWindowLong(activeWindow,GWL_USERDATA); 

		return gadget;
	}
	else
	{
		return 0;
	}
}

EXPORT void activateGadget(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->makeActive();
}

EXPORT CGadget* getParentGadget(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return convertWinHandle(GetParent(gadget->getHwnd()));
}


//Helper Functions

//Return a pointer to the gadget under the mouse
EXPORT CGadget* gadgetUnderMouse()
{
	//Get current cursor position
	POINT mousePos;
	GetCursorPos(&mousePos);

	//Get window at position
	HWND windowUnderMouse=WindowFromPoint(mousePos);

	//Is there a valid window under the mouse?
	if (windowUnderMouse)
	{
		//Get gadget pointer from window handle
		CGadget* result=convertWinHandle(windowUnderMouse);

		//Returns null if window is not a gadget
		return result;
	}
	else
	{
		return NULL;
	}
}

EXPORT HWND windowHandle(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getHwnd();
}

EXPORT void doGadgetAlign(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->doAlign();
}

EXPORT void setGadgetAlign(CGadget* gadget, int align)
{
	MReturnVoidIfNull(gadget)

	gadget->setAlign((ALIGN_TYPE)align);
}

EXPORT void setAutoUpdate(CGadget* gadget, DWORD autoUpdate)
{
	MReturnVoidIfNull(gadget)

	if (autoUpdate)
	{
		gadget->setAutoUpdate(true);
	}
	else
	{
		gadget->setAutoUpdate(false);
	}
}

//TODO - Special handling for canvas gadgets
EXPORT int getTextWidth(CGadgetFont* font, char* text)
{
	//MReturnIfNull(font)
	SIZE textSize=getTextDimensions(font,text);
	return textSize.cx;
}

EXPORT int getTextHeight(CGadgetFont* font, char* text)
{
	//MReturnIfNull(font)
	SIZE textSize=getTextDimensions(font,text);
	return textSize.cy;
}

SIZE getTextDimensions(CGadgetFont* font, char* text)
{
	SIZE	textSize;
	HDC		gadgetDC=GetDC(CApplication::core->hWnd);
	
	//Select the default GUI font into the gadget, this ensures that we get an accurate size returned
	if (font)
	{
		SelectObject(gadgetDC,font->getHandle());
	}
	else
	{
		SelectObject(gadgetDC,GetStockObject(DEFAULT_GUI_FONT));
	}
	
	//Get text dimensions
	GetTextExtentPoint32(gadgetDC,text,strlen(text),&textSize);

	ReleaseDC(CApplication::core->hWnd,gadgetDC);

	return textSize;
}

EXPORT HWND gadgetHandle(CGadget* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getHwnd();
}

//--Special Gadget Functions--

//Panel Functions
//===============
EXPORT void setPanelBorderStyle(CGadgetStatic* gadget,int borderStyle)
{
	MReturnVoidIfNull(gadget)

	gadget->setBorderStyle((PANEL_BORDER_STYLE)borderStyle);
}

EXPORT void setPanelImage(CGadgetStatic* gadget, char* imageFileName)
{
	MReturnVoidIfNull(gadget)

	gadget->loadImageFromFile(imageFileName);
	gadget->applyImage();
}

EXPORT void setPanelImageFromMem(CGadgetStatic* gadget, int imageIndex)
{
	MReturnVoidIfNull(gadget)

	//gadget->sendMsg(STM_SETIMAGE,0,0);
	gadget->loadImageFromMem(imageIndex);
	gadget->applyImage();
}

//Button Functions
//===============
EXPORT void setButtonImage(CGadgetButton* gadget, char* imageFileName)
{
	MReturnVoidIfNull(gadget)

	gadget->loadImageFromFile(imageFileName);
	gadget->applyImage();
}

EXPORT void setButtonImageFromMem(CGadgetButton* gadget, int imageIndex)
{
	MReturnVoidIfNull(gadget)

	gadget->loadImageFromMem(imageIndex);
	gadget->applyImage();
}

//Edit Functions
//===============

/*
	COMMAND_HELP

	setSelText handle gadget, string text

	Replaces the selected text in an edit gadget.  Text can be set in code using setSelStart and setSelLen, and then
	replaced using setSelText.

	gadget:  Handle of an edit or richedit gadget
	text:  The new text which will replace the currently selected text.
*/
EXPORT void setSelText(CGadgetEdit* gadget, char* text)
{
	MReturnVoidIfNull(gadget)

	gadget->setSelText(text);
}

/*
EXPORT DWORD getSelText(DWORD oldText, CGadgetEdit* gadget, char* text)
{
	MReturnVoidIfNull(gadget)

	deleteString(oldText);

	return returnString(gadget->getSelText());
}
*/	

/*
	COMMAND_HELP

	setWordWrapMode handle gadget, bool wordWrap

	Enables or disables word wrapping for a rich-edit gadget.  If word-wrapping is disabled,
	the control can be scrolled horizontally to reveal text which extends beyond the visible area.

	Word wrapping for ordinary edit gadgets can only be set at creation time by specifying the 
	word-wrap flag (4) in the settings parameter of createEdit.

	gadget:  Handle of a rich-edit gadget
	wordWrap:  1 to enable word-wrapping, 0 to disable word-wrapping.
*/
EXPORT void setWordWrapMode(CGadgetRichEdit* gadget, int wordWrap)
{
	MReturnVoidIfNull(gadget)

	gadget->setWordWrapMode(wordWrap);
}

EXPORT void setReadOnly(CGadgetEdit* gadget, bool readOnly)
{
	MReturnVoidIfNull(gadget)

	gadget->setReadOnly(readOnly);
}

EXPORT void setSelStart(CGadgetEdit* gadget, int selStart)
{
	MReturnVoidIfNull(gadget)

	//Note, always sets length of selected text to 0
	gadget->setSel(selStart,0);
}

EXPORT void setSelLen(CGadgetEdit* gadget, int selLength)
{
	MReturnVoidIfNull(gadget)

	gadget->setSel(gadget->getSelStart(),selLength);
}

EXPORT int getReadOnly(CGadgetEdit* gadget)
{
	MReturnIfNull(gadget)

	return (int)gadget->getReadOnly();
}

EXPORT int getSelStart(CGadgetEdit* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getSelStart();
}

EXPORT int getSelLen(CGadgetEdit* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getSelLen();
}


//Checkbox Functions
//===============

EXPORT void setChecked(CGadget* gadget, bool checked)
{
	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_CHECKBOX)
	{
		((CGadgetCheckbox*)(gadget))->setChecked(checked);
	}
	else
	{
		((CGadgetRadiobutton*)(gadget))->setChecked(checked);
	}
}
EXPORT int  getChecked(CGadgetCheckbox* gadget)
{
	MReturnIfNull(gadget)

	return (int)gadget->getChecked();
}

//Listbox/Combobox Functions
//==========================

EXPORT void addItem(CGadget* gadget, char* caption)
{
	terminateApp();

	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		((CGadgetListbox*)(gadget))->addItem(caption);
	//	return;
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		((CGadgetCombobox*)(gadget))->addItem(caption);
	//	return;
	}

	
}

EXPORT void addItem_index(CGadget* gadget, char* caption, int index)
{
	terminateApp();

	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		((CGadgetListbox*)(gadget))->addItem(caption, index);
	//	return;
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		((CGadgetCombobox*)(gadget))->addItem(caption, index);
	//	return;
	}

	
}

EXPORT void removeItem(CGadget* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		((CGadgetListbox*)(gadget))->removeItem(index);
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		((CGadgetCombobox*)(gadget))->removeItem(index);
	}
}

EXPORT void changeItem(CGadget* gadget, int index, char* newCaption)
{
	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		((CGadgetListbox*)(gadget))->changeItem(index, newCaption);
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		((CGadgetCombobox*)(gadget))->changeItem(index, newCaption);
	}
}

EXPORT void selectItem(CGadget* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		((CGadgetListbox*)(gadget))->selectItem(index);
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		((CGadgetCombobox*)(gadget))->selectItem(index);
	}
}

EXPORT int selectedItem(CGadget* gadget)
{
	MReturnIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		return ((CGadgetListbox*)(gadget))->selectedItem();
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		return ((CGadgetCombobox*)(gadget))->selectedItem();
	}

	return 0;
}
//TODO - More graceful handling of an invalid index
EXPORT DWORD itemText(DWORD oldString, CGadget* gadget, int index)
{
	MReturnIfNull(gadget)
	
	int count=0;

   if (gadget->getType()==GADGET_LISTBOX)
   {
	   count=((CGadgetListbox*)(gadget))->getItemCount();
   }
   
   if (gadget->getType()==GADGET_COMBOBOX)
   {
	   count=((CGadgetCombobox*)(gadget))->getItemCount();
   }

	//Return if index is invalid
	if ((index >= count) || (index < 0))
	{
		return 0;
	}

	deleteString(oldString);

	char* itemText=0;

	if (gadget->getType()==GADGET_LISTBOX)
	{
		itemText=((CGadgetListbox*)(gadget))->getItemText(index); //getItemText allocates memory for string
	}

	if (gadget->getType()==GADGET_COMBOBOX)
	{
		itemText=((CGadgetCombobox*)(gadget))->getItemText(index);
	}

	DWORD outString=returnString(itemText); //returnString deletes itemText

	return outString;
}


EXPORT DWORD itemCount(CGadget* gadget)
{
	MReturnIfNull(gadget)

	if (gadget->getType()==GADGET_LISTBOX)
	{
		return ((CGadgetListbox*)(gadget))->getItemCount();
	}
	if (gadget->getType()==GADGET_COMBOBOX)
	{
		return ((CGadgetCombobox*)(gadget))->getItemCount();
	}

	return 0;
}

//Tab Functions
//=============

EXPORT void addTab(CGadgetTabs* gadget, char* caption)
{
	MReturnVoidIfNull(gadget)

	gadget->addTab(caption);
}

EXPORT void addTab_index(CGadgetTabs* gadget, char* caption, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->addTab(caption, index);
}

EXPORT void removeTab(CGadgetTabs* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->removeTab(index);
}

EXPORT void changeTab(CGadgetTabs* gadget, int index, char* newCaption)
{
	MReturnVoidIfNull(gadget)

	gadget->changeTab(index, newCaption);
}

EXPORT int selectedTab(CGadgetTabs* gadget)
{
	MReturnIfNull(gadget)

	return gadget->selectedTab();
}

EXPORT void selectTab(CGadgetTabs* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->setTab(index);
}

EXPORT DWORD tabCaption(DWORD oldString, CGadgetTabs* gadget, int index)
{
	MReturnIfNull(gadget)

	deleteString(oldString);

	char* text=gadget->getTabCaption(index);

	DWORD outString=returnString(text);
	
	return outString;
}

EXPORT int tabCount(CGadgetTabs* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getTabCount();
}

//Statusbar Functions
//===================

EXPORT void addStatusPanel(CGadgetStatusbar* gadget, int width)
{
	MReturnVoidIfNull(gadget)

	gadget->addPanel(width,false);
}

EXPORT void setStatusText(CGadgetStatusbar* gadget, int panelIndex, char* caption)
{
	MReturnVoidIfNull(gadget)

	gadget->setPanelText(panelIndex, caption);
}

EXPORT void removeStatusPanel(CGadgetStatusbar* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->removePanel((DWORD)index);
}

EXPORT int statusPanelCount(CGadgetStatusbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getPanelCount();
}

EXPORT DWORD getStatusText(DWORD oldString, CGadgetStatusbar* gadget, int index)
{
	MReturnIfNull(gadget)

	deleteString(oldString);

	char* text=gadget->getPanelText(index);

	DWORD outString=returnString(text);

	return outString;
}
	
//Scrollbar Functions
//===================

EXPORT void setScrollbarRange(CGadgetScrollbar* gadget, int min, int max)
{
	MReturnVoidIfNull(gadget)

	gadget->setRange(min,max);
}

EXPORT void setScrollbarPosition(CGadgetScrollbar* gadget, int position)
{
	MReturnVoidIfNull(gadget)

	gadget->setPosition(position);
}

EXPORT int getScrollbarPosition(CGadgetScrollbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getPosition();
}

EXPORT int getScrollbarMin(CGadgetScrollbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMinimumValue();
}

EXPORT int getScrollbarMax(CGadgetScrollbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMaximumValue();
}

EXPORT void setScrollbarThumbSize(CGadgetScrollbar* gadget, int size)
{
	MReturnVoidIfNull(gadget)

	gadget->setThumbSize(size);
}

EXPORT int getScrollbarThumbSize(CGadgetScrollbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getThumbSize();
}

//UpDown Functions
//================

EXPORT void setUpDownRange(CGadgetUpDown* gadget, int min, int max)
{
	MReturnVoidIfNull(gadget)

	gadget->setRange(min,max);
}

EXPORT void setUpDownPosition(CGadgetUpDown* gadget, int position)
{
	MReturnVoidIfNull(gadget)

	gadget->setPosition(position);
}

EXPORT int getUpDownPosition(CGadgetUpDown* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getPosition();
}

EXPORT int getUpDownMin(CGadgetUpDown* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMinimumValue();
}

EXPORT int getUpDownMax(CGadgetUpDown* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMaximumValue();
}

//Toolbar Functions
//==================

EXPORT void addStandardToolbarButtons(CGadgetToolbar* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->addStandardButtons();

}

EXPORT void addToolbarButton_index(CGadgetToolbar* gadget, int index, int makeCheckButton)
{
	MReturnVoidIfNull(gadget)

	if (makeCheckButton)
	{
		gadget->addButton(index,true,0);
	}
	else
	{
		gadget->addButton(index,false,0);
	}
}

EXPORT void addToolbarButton_image(CGadgetToolbar* gadget, char* fileName, int makeCheckButton)
{
	MReturnVoidIfNull(gadget)

	if (makeCheckButton)
	{
		gadget->addButton(fileName, true,0);
	}
	else
	{
		gadget->addButton(fileName,false,0);
	}
}

EXPORT void addToolbarButton_index_text(CGadgetToolbar* gadget, int index, int makeCheckButton, char* tooltipText)
{
	MReturnVoidIfNull(gadget)

	if (makeCheckButton)
	{
		gadget->addButton(index,true,tooltipText);
	}
	else
	{
		gadget->addButton(index,false,tooltipText);
	}
}

EXPORT void addToolbarButton_image_text(CGadgetToolbar* gadget, char* fileName, int makeCheckButton, char* tooltipText)
{
	MReturnVoidIfNull(gadget)

	if (makeCheckButton)
	{
		gadget->addButton(fileName, true, tooltipText);
	}
	else
	{
		gadget->addButton(fileName,false, tooltipText);
	}
}

EXPORT void removeToolbarButton(CGadgetToolbar* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->removeButton(index);
}

EXPORT void setToolbarButtonEnabled(CGadgetToolbar* gadget, int index, int enabled)
{
	MReturnVoidIfNull(gadget)

	gadget->setButtonEnabled(index,enabled);
}

EXPORT void setToolbarButtonState(CGadgetToolbar* gadget, int index, int state)
{
	MReturnVoidIfNull(gadget)

	gadget->setButtonState(index,state);
}

EXPORT int getToolbarButtonEnabled(CGadgetToolbar* gadget, int index)
{
	MReturnIfNull(gadget)

	return gadget->getButtonEnabled(index);
}

EXPORT int getToolbarButtonState(CGadgetToolbar* gadget, int index)
{
	MReturnIfNull(gadget)

	return gadget->getButtonState(index);
}

EXPORT int getToolbarItem(CGadgetToolbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getHotItem();
}

EXPORT void addToolbarSplitter(CGadgetToolbar* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->addSplitter();
}

//Menu Commands
EXPORT CGadgetMenu* createMenu(CGadget* parent)
{
	CGadgetMenu* newMenu = new CGadgetMenu;

	HWND parentWindow=0;

	if (parent)
	{
		parentWindow=parent->getHwnd();
	}
	else
	{
#ifdef DBPRO_DLL
		parentWindow=CApplication::core->hWnd;
#endif
	}

	newMenu->create();
	newMenu->attachToWindow(parentWindow);

	return newMenu;
}

EXPORT CGadgetMenu* createPopupMenu()
{
	CGadgetMenu* newMenu = new CGadgetMenu;

	newMenu->createPopup();

	return newMenu;
}

EXPORT CGadgetMenu* createSubMenu(CGadgetMenu* parent, int item)
{
	MReturnIfNull(parent)
	SetLastError(0);
	CGadgetMenu* newMenu = new CGadgetMenu;
	newMenu->create();
	newMenu->attachToMenu(parent->getHMenu(), item);
	return newMenu;
}

EXPORT void deleteMenu_noReturn(CGadgetMenu* menu)
{
	MReturnVoidIfNull(menu)
	menu->destroy();
	delete menu;
}

EXPORT int deleteMenu(CGadgetMenu* menu)
{
	MReturnIfNull(menu)
	menu->destroy();
	delete menu;

	return 0;
}
	

EXPORT void addMenuItem(CGadgetMenu* menu, char* caption, int returnValue)
{
	terminateApp();

	MReturnVoidIfNull(menu)

	menu->addItem(caption,returnValue);
}

EXPORT void addMenuItem_insert(CGadgetMenu* menu, char* caption, int returnValue, int insertBefore)
{
	MReturnVoidIfNull(menu)

	menu->addItem(caption,returnValue,insertBefore);
}

EXPORT void addMenuItem_insert_image(CGadgetMenu* menu, char* caption, int returnValue, int insertBefore, char* image)
{
	MReturnVoidIfNull(menu)

	menu->addItem(caption,returnValue,insertBefore,image);
}

EXPORT void addMenuItem_image(CGadgetMenu* menu, char* caption, int returnValue, char* image)
{
	MReturnVoidIfNull(menu)

	menu->addItem(caption,returnValue,-1,image);
}

EXPORT void addMenuSplitter(CGadgetMenu* menu)
{
	MReturnVoidIfNull(menu)

	menu->addSplitter();
}

EXPORT void setMenuItemChecked(CGadgetMenu* menu, int item, int boolChecked)
{
	MReturnVoidIfNull(menu)

	menu->setItemChecked(item, boolChecked);
}

EXPORT void setMenuItemEnabled(CGadgetMenu* menu, int item, int boolEnabled)
{
	MReturnVoidIfNull(menu)

	menu->setItemEnabled(item, boolEnabled);
}

EXPORT int getMenuItemChecked(CGadgetMenu* menu, int item)
{
	MReturnIfNull(menu)

	return menu->getItemChecked(item);

}

EXPORT int getMenuItemEnabled(CGadgetMenu* menu, int item)
{
	MReturnIfNull(menu)

	return menu->getItemEnabled(item);
}

EXPORT int showPopupMenu(CGadgetMenu* menu)
{
	MReturnIfNull(menu)

	POINT mousePos;

	GetCursorPos(&mousePos);

	return menu->showPopup(mousePos.x,mousePos.y);
}

EXPORT DWORD getMenuItemText(DWORD oldString, CGadgetMenu* menu, int item)
{
	if (!menu)
	{
		deleteString(oldString);
		return 0;
	}

	char* itemText = new char[255];
	menu->getItemText(item, itemText, 255);

	deleteString(oldString);
	return returnString(itemText);
}

EXPORT void setMenuItemText(CGadgetMenu* menu, int item, char* newCaption)
{
	MReturnVoidIfNull(menu)

	menu->setItemText(item,newCaption);
}

EXPORT int getMenuItemCount(CGadgetMenu* menu)
{
	MReturnIfNull(menu)

	return menu->getItemCount();
}

EXPORT void setMenuItemRadio(CGadgetMenu* menu, int firstGroupItem, int lastGroupItem, int selectItem)
{
	MReturnVoidIfNull(menu)
	
	menu->setItemRadioChecked(selectItem, firstGroupItem, lastGroupItem);
}

//Trackbar Gadgets
//================

EXPORT void setTrackbarRange(CGadgetTrackbar* gadget, int min, int max)
{
	MReturnVoidIfNull(gadget)

	gadget->setRange(min,max);
}

EXPORT void setTrackbarTickFrequency(CGadgetTrackbar* gadget, int frequency)
{
	MReturnVoidIfNull(gadget)

	gadget->setTickFrequency(frequency);
}

EXPORT void setTrackbarPosition(CGadgetTrackbar* gadget, int position)
{
	MReturnVoidIfNull(gadget)

	gadget->setPosition(position);
}

EXPORT int getTrackbarMin(CGadgetTrackbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMinimumValue();
}

EXPORT int getTrackbarMax(CGadgetTrackbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMaximumValue();
}

EXPORT int getTrackbarPosition(CGadgetTrackbar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getPosition();
}

//Canvas Functions
//================

EXPORT void setDrawingColor(CGadgetCanvas* gadget, DWORD lineColor, DWORD fillColor, DWORD textColor)
{
	MReturnVoidIfNull(gadget)

	gadget->setDrawingColor(convertColor(lineColor),convertColor(fillColor),convertColor(textColor));
}

EXPORT void setLineWidth(CGadgetCanvas* gadget, int width)
{
	MReturnVoidIfNull(gadget)

	gadget->setLineWidth(width);
}

EXPORT void copyCanvas(CGadgetCanvas* gadget, CGadgetCanvas* source, int left, int top, int right, int bottom)
{
	MReturnVoidIfNull(gadget)
	MReturnVoidIfNull(source)

	gadget->copyFrom(source,left,top,right,bottom);
}

EXPORT void drawLine(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2)
{
	MReturnVoidIfNull(gadget)

	gadget->drawLine(x1,y1,x2,y2);
}

EXPORT void drawRect(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2)
{
	MReturnVoidIfNull(gadget)
	gadget->drawRect(x1,y1,x2,y2);
}

EXPORT void drawEllipse(CGadgetCanvas* gadget, int x1, int y1, int x2, int y2)
{
	MReturnVoidIfNull(gadget)
	gadget->drawEllipse(x1,y1,x2,y2);
}

EXPORT void drawText(CGadgetCanvas* gadget, int x, int y, char* text)
{
	MReturnVoidIfNull(gadget)

	gadget->drawText(x,y,text);
}

EXPORT void drawPoint(CGadgetCanvas* gadget, int x, int y, DWORD color)
{
	MReturnVoidIfNull(gadget)
	gadget->drawPoint(x,y,convertColor(color));
}

//Listview Functions
//==================

EXPORT void setListViewImageList(CGadgetListview* gadget, CGadgetImageList* images)
{
	gadget->setImageList(images,LVSIL_NORMAL);
}

EXPORT void setListViewImageList_withIndex(CGadgetListview* gadget, CGadgetImageList* images, int listIndex)
{
	gadget->setImageList(images, listIndex);
}

EXPORT void setListViewStyle(CGadgetListview* gadget, int style)
{
	gadget->setViewStyle(style);
}

EXPORT void addListViewItem(CGadgetListview* gadget, char* caption, int image)
{
	MReturnVoidIfNull(gadget)
	gadget->addItem(caption, image);
}

EXPORT void addListViewColumn(CGadgetListview* gadget, char* caption, int width)
{
	MReturnVoidIfNull(gadget)
	gadget->addColumn(caption, width);
}

EXPORT int getListViewItemCount(CGadgetListview* gadget)
{
	MReturnIfNull(gadget)
	return gadget->getItemCount();
}

EXPORT void setListViewItemText(CGadgetListview* gadget, int index, int subItemIndex, char* caption)
{
	MReturnVoidIfNull(gadget)
	gadget->setItemText(caption, index, subItemIndex);
}

EXPORT DWORD getListViewItemText(DWORD pOldString, CGadgetListview* gadget, int item, int subItem)
{
	MReturnIfNull(gadget)

	deleteString(pOldString);

	char* buffer=new char[255];

	gadget->getItemText(item,subItem,buffer,255);

	return returnString(buffer);
}

EXPORT void selectListViewItem(CGadgetListview* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->selectItem(index);
}

EXPORT int selectedListViewItem(CGadgetListview* gadget)
{
	MReturnIfNull(gadget)

	return gadget->selectedItem();
}

EXPORT void removeListViewItem(CGadgetListview* gadget, int index)
{
	MReturnVoidIfNull(gadget)

	gadget->removeItem(index);
}



//ImageList Functions
//===================

EXPORT CGadgetImageList* createImageList(int width, int height)
{
	CGadgetImageList*	newImageList=new CGadgetImageList;

	newImageList->create(width,height,255);

	return newImageList;
}

EXPORT void addImageListItem(CGadgetImageList* imageList, char* fileName)
{
	MReturnVoidIfNull(imageList)
	imageList->addImage(fileName);
}

EXPORT void addImageListItem_FromMem(CGadgetImageList* imageList, int imageIndex)
{
	MReturnVoidIfNull(imageList)
	imageList->addImage(imageIndex);
}

/*
	COMMAND_HELP

	deleteImageList handle imageList

	Deletes an image list.

	imageList:  The image list to delete.
*/
EXPORT void deleteImageList(CGadgetImageList* imageList)
{
	MReturnVoidIfNull(imageList);
	delete imageList;
}

/* 
	COMMAND_HELP
	
	removeImageListItem handle imageList, int imageIndex

	Removes an item from an image list.  

	imageList:  Handle of an image list from which an item should be removed
	imageIndex: Index of the image which should be removed from the list
*/
EXPORT void removeImageListItem(CGadgetImageList* imageList, int imageIndex)
{
	MReturnVoidIfNull(imageList)
	imageList->removeImage(imageIndex);
}

//Treeview Functions
//===================

EXPORT HTREEITEM addTreeViewItem(CGadgetTreeview* gadget, HTREEITEM parent, int image, char* caption)
{
	MReturnIfNull(gadget)

	return gadget->addItem(parent,image,caption);
}

EXPORT HTREEITEM addTreeViewItem_index(CGadgetTreeview* gadget, HTREEITEM parent, int image, char* caption, HTREEITEM insertAfter)
{
	MReturnIfNull(gadget)

	return gadget->addItem(parent,image,caption,insertAfter);
}

EXPORT void setTreeViewImageList(CGadgetTreeview* gadget, CGadgetImageList* imageList, int index)
{
	MReturnVoidIfNull(gadget)
	MReturnVoidIfNull(imageList)

	gadget->setImageList(imageList,index);
}

EXPORT void removeTreeViewItem(CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnVoidIfNull(gadget)
	
	gadget->removeItem(item);
}

EXPORT void selectTreeViewItem(CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnVoidIfNull(gadget)
	gadget->selectItem(item);
}

EXPORT HTREEITEM selectedTreeViewItem(CGadgetTreeview* gadget)
{
	MReturnIfNull(gadget)

	return gadget->selectedItem();
}

EXPORT HTREEITEM treeViewItemFromPoint(CGadgetTreeview* gadget, int x, int y)
{
	MReturnIfNull(gadget)

	return gadget->hitTest(x,y);
}

EXPORT void setTreeViewItemText(CGadgetTreeview* gadget, HTREEITEM item, char* text)
{
	MReturnVoidIfNull(gadget)

	gadget->setItemText(item,text);
}

EXPORT DWORD getTreeViewItemText(DWORD oldString, CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnIfNull(gadget)

	deleteString(oldString);

	char* itemText = new char[255];
	gadget->getItemText(item, itemText, 255);
	
	return returnString(itemText);
}

EXPORT HTREEITEM getTreeViewChild(CGadgetTreeview* gadget, HTREEITEM parentItem)
{
	MReturnIfNull(gadget)

	return gadget->getFirstChild(parentItem);
}

EXPORT HTREEITEM getTreeViewRoot(CGadgetTreeview* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getRoot();
}

EXPORT HTREEITEM getTreeViewNext(CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnIfNull(gadget)
	
	return gadget->getNextSibling(item);
}

EXPORT HTREEITEM getTreeViewPrev(CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnIfNull(gadget)

	return gadget->getPreviousSibling(item);
}

EXPORT HTREEITEM getTreeViewParent(CGadgetTreeview* gadget, HTREEITEM item)
{
	MReturnIfNull(gadget)

	return gadget->getParent(item);
}

//WebBrowser
//==========

EXPORT void webBrowserNavigate(CGadgetWebBrowser* gadget, char* URL)
{
	MReturnVoidIfNull(gadget)
	gadget->navigate(URL);
}

EXPORT DWORD getWebBrowserURL(DWORD oldString, CGadgetWebBrowser* gadget)
{
	MReturnIfNull(gadget)

	deleteString(oldString);

	char* buffer = new char[255];
	ZeroMemory(buffer,255);

	gadget->currentURL(buffer,255);
	
	return returnString(buffer);
}

EXPORT void webBrowserRun(CGadgetWebBrowser* gadget, char* html)
{
	MReturnVoidIfNull(gadget)

	gadget->run(html);
}

//Font
//====
EXPORT CGadgetFont* createFont(char* name, int size, int bold, int italic, int underline)
{
	CGadgetFont* newFont=new CGadgetFont;

	if (newFont->create(name,size,bold,italic,underline))
	{
		return newFont;
	}
	else
	{
		return 0;
	}
}

EXPORT void deleteFont(CGadgetFont* font)
{
	MReturnVoidIfNull(font);

	font->destroy();
	delete font;
}

EXPORT void applyFont(CGadget* gadget, CGadgetFont* font)
{
	MReturnVoidIfNull(font);

	font->apply(gadget);
}

//Window

EXPORT void maximizeWindow(CGadgetWindow* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->maximize();
}

EXPORT void minimizeWindow(CGadgetWindow* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->minimize();
	
}

EXPORT void restoreWindow(CGadgetWindow* gadget)
{
	MReturnVoidIfNull(gadget)

	gadget->restore();
}

EXPORT void setWindowIcon(CGadgetWindow* gadget, char* fileName)
{
	MReturnVoidIfNull(gadget)

	gadget->setIcon(fileName);
}

EXPORT void setGadgetShape(CGadget* gadget, char* fileName)
{
	MReturnVoidIfNull(gadget)

	gadget->setShape(fileName);
}


/*
	COMMAND_HELP

	setGadgetColor handle gadget, color background, color text

	Sets the background and text colour of gadgets which support custom-colours.
	The following gadgets currently support custom colours:

	- Button
	- Label
	- Panel
	- Edit
	- RichEdit (background colour only)
	- Checkbox
	- RadioButton
	- ListBox
	- ComboBox	

	The colour values can be generated using the RGB() function.
	
	gadget:  Handle of a gadget
	background:  Colour to apply to the background of the gadget
	text:  Colour to apply to the gadget's text.
	
*/
EXPORT void setGadgetColor(CGadget* gadget, DWORD background, DWORD text)
{
	MReturnVoidIfNull(gadget)

	gadget->setColors(convertColor(background),convertColor(text));
}

EXPORT DWORD getPoint(CGadgetCanvas* gadget, int x, int y)
{
	MReturnIfNull(gadget)

	return convertColor(gadget->getPoint(x,y));
}

EXPORT void getCanvasImage(CGadgetCanvas* gadget, int imageId)
{
	MReturnVoidIfNull(gadget)

	gadget->getImage(imageId);
}

EXPORT void setGadgetTextAlign(CGadget* gadget,DWORD textAlignment)
{
	MReturnVoidIfNull(gadget)

	gadget->alignText((TEXT_ALIGN)textAlignment);
}

//Progress Bar
EXPORT void setProgressBarRange(CGadgetProgressBar* gadget, int minV, int maxV)
{
	MReturnVoidIfNull(gadget)

	gadget->setRange(minV,maxV);
}

EXPORT void setProgressBarPos(CGadgetProgressBar* gadget, int pos)
{
	MReturnVoidIfNull(gadget)

	gadget->setPosition(pos);
}

EXPORT int getProgresBarMin(CGadgetProgressBar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMin();
}

EXPORT int getProgressBarMax(CGadgetProgressBar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getMax();
}

EXPORT int getProgressBarPos(CGadgetProgressBar* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getPosition();
}

//RichEdit Gadgets
EXPORT void setSelFont(CGadgetRichEdit* gadget, CGadgetFont* font, DWORD color)
{
	MReturnVoidIfNull(gadget)

	gadget->setSelFont(font,convertColor(color));
}

EXPORT CGadgetFont* getSelFont(CGadgetRichEdit* gadget)
{
	MReturnIfNull(gadget)

	return gadget->getSelFont();
}

EXPORT COLORREF getSelColor(CGadgetRichEdit* gadget)
{
	MReturnIfNull(gadget)

	return convertColor(gadget->getSelColor());
}

EXPORT DWORD getFontName(DWORD pOldString, CGadgetFont* font)
{
	MReturnIfNull(font)

	deleteString(pOldString);

	char* strPtr=font->getFace();

	return returnString(strPtr);
}

EXPORT int getFontSize(CGadgetFont* font)
{
	MReturnIfNull(font)

	return font->getSize();
}

EXPORT int isFontBold(CGadgetFont* font)
{
	MReturnIfNull(font)

	return font->isBold();
}

EXPORT int isFontItalic(CGadgetFont* font)
{
	MReturnIfNull(font)

	return font->isItalic();

}

EXPORT int isFontUnderlined(CGadgetFont* font)
{
	MReturnIfNull(font)

	return font->isUnderlined();
}

EXPORT void setGadgetAlpha(CGadget* gadget, int alpha)
{
	MReturnVoidIfNull(gadget)

	gadget->setAlpha(alpha);
}

EXPORT int listViewItemFromPoint(CGadgetListview* gadget, int x, int y)
{
	MReturnIfNull(gadget)

	return gadget->hitTestItem(x,y);
}

EXPORT int listViewSubItemFromPoint(CGadgetListview* gadget, int x, int y)
{
	MReturnIfNull(gadget)

	return gadget->hitTestSubItem(x,y);
}

EXPORT int listViewItemX(CGadgetListview* gadget, int item, int subItem)
{
	MReturnIfNull(gadget)

	return gadget->getItemX(item,subItem);
}

EXPORT int listViewItemY(CGadgetListview* gadget, int item, int subItem)
{
	MReturnIfNull(gadget)

	return gadget->getItemY(item,subItem);
}

EXPORT void setListViewColumnWidth(CGadgetListview* gadget, int column, int width)
{
	MReturnVoidIfNull(gadget)

	gadget->setColumnWidth(column,width);
}

EXPORT int getListViewColumnWidth(CGadgetListview* gadget, int column)
{
	MReturnIfNull(gadget)

	return gadget->getColumnWidth(column);
}

EXPORT void setGadgetDefaults(BOOL visible)
{
	if (visible)
	{
		CGadget::defaultStyles |= WS_VISIBLE;
	}
	else
	{
		CGadget::defaultStyles -= (CGadget::defaultStyles&WS_VISIBLE);
	}
}

/*
	COMMAND_HELP

	enableXPStyles handle gadget,bool xpStyles

	Enables or disables XP visual styles for a specific gadget.
	Whether or not the application uses visual styles when running under Windows XP depends on whether or not 
	there is an appropriate manifest file in the application directory (see the BlueGUI Guide).

	gadget:  Handle of a gadget.
	xpStyles: 1 to enable themes for the gadget, 0 to disable visual styles for the gadget.

*/
EXPORT void enableXPStyles(CGadget* gadget, int xpStyles)
{
	MReturnVoidIfNull(gadget)

	if (xpStyles)
		gadget->enableXPStyle(true);
	else
		gadget->enableXPStyle(false);
}

/*
	COMMAND_HELP

	setGadgetTab handle gadget, int focus, handle insertAfter

	Sets whether or not a gadget can be focused using the tab key, and the tab order for gadgets
	with the same parent.

	gadget:  Handle of a gadget
	focus:  1 to allow the user to focus the gadget using the tab key, or 0 to disable tab-focusing.
	insertAfter:	Handle of the gadget which will preceed this gadget in the tab order.  When the user \ 
					is tabbing through gadgets in a window, this gadget will be focused after the insertAfter gadget.
*/
EXPORT void setGadgetTab(CGadget* gadget, int focus, CGadget* insertAfter)
{
	MReturnVoidIfNull(gadget)

	bool enableTabFocus=(focus != 0);
	gadget->setTabFocus(enableTabFocus);

	if (insertAfter)
	{
		gadget->setTabOrder(insertAfter);
	}
}

/*  
	COMMAND_HELP

	setDialogOwner handle gadget

	Sets the gadget which will own any dialogs shown subsequently.  Usually file, colour, font etc. dialogs
	are children of the main DarkBASIC Pro application window.  If you want them to be parented to a window created
	using createWindow, you need to use this command before displaying the dialog.

	gadget:  Handle of the gadget that will own dialogs which are shown subsequently
*/
EXPORT void setDialogOwner(CGadget* gadget)
{
	if (gadget)
	{
		dialogOwnerWindow=gadget->getHwnd();
	}
	else
	{
		dialogOwnerWindow=CApplication::core->hWnd;
	}

}

/*  
	COMMAND_HELP

	attachGadget handle target, int tabPage, handle attach

	Attaches a gadget to a 'page' of a tab gadget.  This means that when the user selects
	the tab, the attached gadget will be shown, and when the user selects a different tab, the attached
	gadget will be hidden.
	This allows you to easily create a tab-based dialog where different gadgets are shown depending on which tab
	is selected.  

	If a gadget is attached to a tab, it must be detached using detachGadget before it is deleted.

	target:  Handle of the tab gadget
	tabPage:  The index of the tab which the gadget is to be attached to
	attach:  The handle of the gadget which is to be attached.
*/
EXPORT void attachGadget(CGadgetTabs* target, int item, CGadget* attach)
{
	MReturnVoidIfNull(attach)
	MReturnVoidIfNull(target)

	target->attachGadget(item,attach);
}
 
/*  
	COMMAND_HELP

	detachGadget handle target, int item, handle detach

	Detaches a gadget from a page on a tab gadget.  This is the opposite of the attachGadget command.  Once
	a gadget has been detached from a tab, its visibility no longer depends on whether or not that gadget is selected.
*/
EXPORT void detachGadget(CGadgetTabs* target, int item, CGadget* detatch)
{
	MReturnVoidIfNull(detatch)
	MReturnVoidIfNull(target)

	target->detatchGadget(item,detatch);
}

/*
EXPORT void setDisplayGadget(CGadget* gadget)
{
	MReturnVoidIfNull(gadget)

		CApplication::paintWindow=gadget->getHwnd();
}
*/
inline void terminateApp()
{
	if (!CApplication::initSuccess)
	{
		MessageBox(0,"BlueGUI Plugin Could Not Load.  Wrong UserID or Key passed to startBlue","Plugin Error",MB_ICONERROR);
			
		TerminateProcess(GetCurrentProcess(),0);
	}
}
