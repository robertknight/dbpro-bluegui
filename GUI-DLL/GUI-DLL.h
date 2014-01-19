//#define _WIN32_WINNT 0x501

#pragma once

//DLL Settings
//=================

//Define DEMO_VERSION to enable time-limit
//#define DEMO_VERSION

#undef WINVER
#undef _WIN32_IE
#undef _WIN32_WINNT

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501

//Define DBPRO_DLL for a DBPro TPC plugin, or GENERIC_DLL otherwise
#define DBPRO_DLL
//#define GENERIC_DLL

//DLL Export Macros
#define EXPORT extern "C" __declspec (dllexport)
#define EXPORT_MANGLED __declspec (dllexport)

#ifdef DEMO_VERSION
	#define DEMO_TIMEOUT (240*1000)
#endif

//Main DLL Includes
//=================

#include <windows.h>
#include "resource.h"
#include "commctrl.h"

#pragma once
#include "web.h"
#include "class_imagelist.h"
#include "class_gadget.h"
#include "class_menu.h"
#include "class_font.h"
#include "dialogs.h"
      

#include "gui_functions.h"

#include <stdio.h>
#include "..\..\DBPro_Includes\globstruct.h"

//Custom Event Names
#define BGUI_MENUCLICK       (WM_USER+1)
#define BGUI_BUTTONCLICK     (WM_USER+2)
#define BGUI_COMBOBOXCHANGE  (WM_USER+3)
#define BGUI_TREEVIEWCHANGE  (WM_USER+4)

//GUIDLL Functions
void		GSetupCommonControls();
void		Crypt(TCHAR* inp, DWORD inplen, TCHAR *key="",DWORD keylen=0);
inline
void		terminateApp();

//Macros

//Returns from a function if the specified object is null
#define MReturnIfNull(object) if (object == 0) { return 0; }

//Returns void from a function if the specified object is null
#define MReturnVoidIfNull(object) if (object == 0) {return;}

//Colour conversions (RGB <> BGR)
DWORD convertColor(DWORD color);

//Custom Gadget Classes
ATOM registerCGadgetWindow();
ATOM registerCGadgetWebBrowser();

LRESULT CALLBACK windowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK browserWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//DBPro function pointer types
typedef void	( *PFMakeMemblockFromImage )	(DWORD, DWORD);		//makeMemblockFromImage(memblockId,imageId)
typedef void	( *PFMakeImageFromMemblock )	(DWORD, DWORD);		//makeImageFromMemblock(imageId,memblockId)
typedef void    ( *PFMakeMemblock)				(DWORD, DWORD);		//makeMemblock(id,size)
typedef DWORD	( *PFMemblockExists)			(DWORD);			//memblockExists(memblockId)
typedef DWORD	( *PFGetMemblockPtr)			(DWORD);			//getMemblockPtr(memblockId)
typedef DWORD	( *PFGetMemblockSize)			(DWORD);			//getMemblockSize(memblockId)
typedef void	( *PFDeleteMemblock)			(DWORD);			//deleteMemblock(memblockId)
typedef DWORD	( *PFImageExists)				(DWORD);			//imageExists(imageId)
typedef DWORD   ( *PFAlphaBlend)				(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
typedef BOOL	(__stdcall *PFSetLayeredWindowAttributes)(HWND,COLORREF,BYTE,DWORD);

typedef void    (*PFAddSwapChain)				(HWND); //addSwapChain(HWND);
typedef void	(*PFStartSwapChain)				(int); //startSwapChain(id);
typedef void	(*PFEndSwapChain)				(void); //endSwapChain(id);
typedef void	(*PFUpdateSwapChain)			(int); //updateSwapChain(id);

//String handling functions
DWORD returnString(LPSTR pStr);
void deleteString(DWORD pOldString);

//Generic window message processing function
LRESULT CALLBACK SubClassFunc(   HWND hWnd,
               UINT Message,
               WPARAM wParam,
               LONG lParam);

//Broadcast function to send message to all child windows with the specified parent
void _stdcall broadcastMessage(HWND parent, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL _stdcall broadcastEnumProc(HWND child, LPARAM userData);

//CApplication class used for sharing important data around application
//There probably is an easier way, but I couldn't figure it out.

class CApplication
{
public:
	//Pointer to DBPro core
	
#ifdef DBPRO_DLL
	static GlobStruct*					core;
#endif

	//DBPro function pointers
	static PFMakeMemblockFromImage		makeMemblockFromImage;
	static PFMakeImageFromMemblock		makeImageFromMemblock;
	static PFMakeMemblock				makeMemblock;
	static PFMemblockExists				memblockExists;
	static PFGetMemblockPtr				getMemblockPtr;
	static PFGetMemblockSize			getMemblockSize;
	static PFDeleteMemblock				deleteMemblock;
	static PFImageExists				imageExists;
	static PFAlphaBlend					alphaBlend;

	static bool							initSuccess;
	
	//Original main window procedure
	static LONG							originalMainWindowProc;

	static HWND							paintWindow;

	//Cursors
	static HCURSOR						sizingCursor;

	//DLL Instance Handle
	static HINSTANCE					hInstance;

	//Handle of web module
	static HMODULE						webLib;


	static DWORD						demoTimer;

	static bool							isWindowsNT;

	static bool							timeout;
};

//Core DLL Function
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
);
