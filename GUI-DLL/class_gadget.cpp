#include "GUI-DLL.h"
#include <assert.h>
#include <richedit.h>
#include <commctrl.h>

//class CApplication;

#ifdef DBPRO_DLL
	GlobStruct*				CApplication::core;
#endif

PFMakeMemblockFromImage CApplication::makeMemblockFromImage;
PFMakeImageFromMemblock CApplication::makeImageFromMemblock;
PFMakeMemblock			CApplication::makeMemblock;
PFMemblockExists		CApplication::memblockExists;
PFGetMemblockPtr		CApplication::getMemblockPtr;
PFGetMemblockSize		CApplication::getMemblockSize;
PFImageExists			CApplication::imageExists;
PFDeleteMemblock		CApplication::deleteMemblock;
LONG					CApplication::originalMainWindowProc;
HCURSOR					CApplication::sizingCursor;
HINSTANCE				CApplication::hInstance;
PFAlphaBlend			CApplication::alphaBlend;
HWND					CApplication::paintWindow;
bool					CApplication::isWindowsNT;

CGadgetButton* CGadgetButton::BoldFocusButton;

//Selects the next gadget in the Z-order
void    focusNextGadget(HWND hWnd);

//Used to manually paint a gadget's background
void	paintBackground(HWND hWnd);

//Processes messages from a scrollbar to update its position
void processScrollbarMessage(LPARAM lParam, WPARAM wParam);

//Filters message types
bool isAllowedMessage(UINT Message);

//CGadgetWebBrowser vars
EmbedBrowserObjectPtr		*lpEmbedBrowserObject;
UnEmbedBrowserObjectPtr		*lpUnEmbedBrowserObject;
DisplayHTMLPagePtr			*lpDisplayHTMLPage;
DisplayHTMLStrPtr			*lpDisplayHTMLStr;
GetCurrentURLPtr			*lpGetCurrentURL;
SetSizePtr					*lpSetSize;
unsigned int WindowCount;


//CGadget
//================================================================================================
CGadget::CGadget()
{
	//This function is called by all classes derived from CGadget:

	//CGadget constructor -> Derived class constructor (if specified)

	//Initialize vars
	gadgetImage						=0;
	align							=ALIGN_NONE;
	disableAutoUpdate				=false;
	requiresManualBackgroundPaint	=false;
	myGadgetType					=GADGET_NONEXISTANT;
	myDragResizeSide				=1;
	gadgetDC						=0;
	myData							=0;
	backgroundBrush					=0;
	textColor						=0;
}

void CGadget::setAutoUpdate(bool autoUpdate)
{
	if (autoUpdate)
	{
		sendMsg(WM_SETREDRAW,TRUE,0);
	}
	else
	{
		sendMsg(WM_SETREDRAW,FALSE,0);
	}
}


void CGadget::setColors(COLORREF background, COLORREF text)
{
	//Delete existing objects
	if (backgroundBrush)	{ DeleteObject(backgroundBrush); }
	
	//Create new objects
	backgroundBrush=CreateSolidBrush(background);
	textColor=text;

	//Update gadget
	update();
}

void CGadget::setAlpha(int alpha)
{
		//Only works for non-child windows

		//Add layered property to gadget if it is not already layered
		SetWindowLong(myHwnd,GWL_EXSTYLE,GetWindowLong(myHwnd,GWL_EXSTYLE)|WS_EX_LAYERED);

		if (CApplication::isWindowsNT)
		{
			HANDLE userLib=GetModuleHandle("User32");

			

			if (userLib)
			{
				PFSetLayeredWindowAttributes myFunc=(PFSetLayeredWindowAttributes)GetProcAddress((HMODULE)userLib,"SetLayeredWindowAttributes");

				

				if (myFunc)
				{
					myFunc(myHwnd,0,(byte)alpha,LWA_ALPHA);
				}
			}

		}
		//GetModuleHandle("User32")
		//Set window alpha
		//SetLayeredWindowAttributes(myHwnd,0,(byte)alpha,LWA_ALPHA);
}

void CGadget::setShape(char* fileName)
{
	HBITMAP regionImage=(HBITMAP)LoadImage(0,fileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	SetWindowRgn(myHwnd,bitmapToRegion(regionImage),TRUE);
}

//Code by Jean-Edouard Lachand Robert
//http://www.codeguru.com/Cpp/G-M/bitmap/usingregions/article.php/c1751/
HRGN CGadget::bitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;

//	ASSERT(hBmp);
	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap conten
		HDC hMemDC = CreateCompatibleDC(NULL);
//		ASSERT(hMemDC);
		if (hMemDC)
		{
			// Get bitmap siz
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize
					bm.bmWidth,					// biWidth;
					bm.bmHeight,				// biHeight;
					1,							// biPlanes;
					32,							// biBitCount
					BI_RGB,						// biCompression;
					0,							// biSizeImage;
					0,							// biXPelsPerMeter;
					0,							// biYPelsPerMeter;
					0,							// biClrUsed;
					0							// biClrImportant;
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
		
			//			ASSERT(hbm32);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory D
				HDC hDC = CreateCompatibleDC(hMemDC);
//				ASSERT(hDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits
					BITMAP bm32;
//					VERIFY(GetObject(hbm32, sizeof(bm32), &bm32));
					GetObject(hbm32,sizeof(bm32),&bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory D
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					//VERIFY(BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY));
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					// For better performances, we will use the ExtCreateRegion() function to create th
					// region. This function take a RGNDATA structure on entry. We will add rectangles b
					// amount of ALLOC_UNIT number in this structure
					#define ALLOC_UNIT	100

					DWORD maxRects = ALLOC_UNIT;
				//	DWORD maxRects = 100;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixel
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to righ
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the regio
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									//VERIFY();
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
									//ASSERT(pData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is to
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									//ASSERT(h);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangle
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					//ASSERT(h);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
						GlobalFree(hData);
						SelectObject(hDC, holdBmp);
						DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}

	return hRgn;
}

DWORD CGadget::customPaint(HDC deviceContext)
{
	if (backgroundBrush)
	{
		//Get background brush colour
		LOGBRUSH brushInfo;
		GetObject(backgroundBrush,sizeof(LOGBRUSH),&brushInfo);

		SetBkColor(deviceContext,brushInfo.lbColor);
		SetTextColor(deviceContext,textColor);	

		return (DWORD)backgroundBrush;
	}
	else
	{
		return 0;
	}
}

void CGadget::setTabOrder(CGadget* insertAfter)
{
	SetWindowPos(myHwnd,insertAfter->getHwnd(),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

//Placeholder function
bool CGadget::create()
{
	return false;
}

//Gadget destruction function
void CGadget::destroy()
{
	DestroyWindow(myHwnd);
}

bool CGadget::createFromWindow(HWND existingWindow)
{
	//If the user has passed a valid handle to an existing window, set the gadget's myHwnd member to the handle 
	//of the existing window and subclass it so that we can interact with it
	if (IsWindow(existingWindow))
	{
		myHwnd=existingWindow;
		
		//Subclass window
#ifdef DBPRO_DLL
		if (myHwnd != CApplication::core->hWnd)
#endif
		{
			//Apply WS_CLIPCHILDREN property so window can have child gadgets in it
			SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|WS_CLIPCHILDREN);

			myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
			
		}
#ifdef DBPRO_DLL
		else
		{
			myOldWinProc=(WNDPROC)CApplication::originalMainWindowProc;
		}
#endif

		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);


		return true;

	}
	else
	{
		return false;
	}
}

ATOM CGadget::registerClass(HINSTANCE hInstance)
{
//	return RegisterClassEx(&myWindowClass);
	return 0;
}

void CGadget::setVisible(DWORD flag,BOOL centerInParent)
{
	// If flag is TRUE, show the window, else hide it
	if (flag)
	{
		//If the centerInParent flag is enabled then move the window before showing it
		if (centerInParent)
		{
			HWND parent=0;

			//Get handle to parent window
			parent=GetParent(myHwnd);

			if (!parent)
			{
				//Or use the desktop window if the gadget is a top-level window
				parent=GetDesktopWindow();
			}

			//Get width and height of parent window
			RECT parentInfo;

			GetWindowRect(parent,&parentInfo);
			
			int parentWidth=(parentInfo.right-parentInfo.left);
			int parentHeight=(parentInfo.bottom-parentInfo.top);

			//Position the gadget so that it is in the center of the parent window
			move((parentWidth/2)-(getWidth()/2),(parentHeight/2)-(getHeight()/2),getWidth(),getHeight(),false);
		}
	
		if (flag==1)
		{
			ShowWindow(myHwnd,SW_SHOW);
		}
		if (flag==2)
		{
			ShowWindow(myHwnd,SW_SHOWNOACTIVATE);
		}
	}
	else
	{
		ShowWindow(myHwnd,SW_HIDE);
	}
}

int CGadget::getVisible()
{
	return IsWindowVisible(myHwnd);
}


/*void CGadget::setClassDefaults()
{

	myWindowClass.cbSize = sizeof(WNDCLASSEX); 

	myWindowClass.style			= CS_HREDRAW | CS_VREDRAW;
	myWindowClass.lpfnWndProc	= (WNDPROC)(0);
	myWindowClass.cbClsExtra		= 0;
	myWindowClass.cbWndExtra		= 0;
	myWindowClass.hInstance		= NULL;
	myWindowClass.hIcon			= NULL;
	myWindowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	myWindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	myWindowClass.lpszMenuName	= NULL;
	myWindowClass.lpszClassName	= "GADGET_CLASS";
	myWindowClass.hIconSm		= NULL;
}*/

void CGadget::setDefaultFont()
{
	//Must be called after gadget has been shown
	//Removes the ugly bold font from the gadget
    
	SendMessage(myHwnd, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
}

void CGadget::bringToFront()
{
	BringWindowToTop(myHwnd);
}

void CGadget::sendToBack()
{
	SetWindowPos(myHwnd,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

void CGadget::move(int xPos, int yPos, int width, int height, bool clientSize)
{
	int sizeOffsetX=0;
	int sizeOffsetY=0;

	//If clientSize is true, the width and height are desired client width and height.  To calculate the X & Y offsets, we 
	//need to work out the sizes of the borders
	if (clientSize)
	{
		//Prepare window info structure & fill it with information

		WINDOWINFO winInfo;

		ZeroMemory(&winInfo,sizeof(WINDOWINFO));
		winInfo.cbSize=sizeof(WINDOWINFO);
		GetWindowInfo(myHwnd,&winInfo);

		//Get client & full dimensions
		int clientWidth=winInfo.rcClient.right-winInfo.rcClient.left;
		int clientHeight=winInfo.rcClient.bottom-winInfo.rcClient.top;
		int fullWidth=winInfo.rcWindow.right-winInfo.rcWindow.left;
		int fullHeight=winInfo.rcWindow.bottom-winInfo.rcWindow.top;

		sizeOffsetX=fullWidth-clientWidth;
		sizeOffsetY=fullHeight-clientHeight;
	}

	//Note - Window is not repainted by this command
	MoveWindow(myHwnd,xPos,yPos,width+sizeOffsetX,height+sizeOffsetY,true);

	onSize(width , height);
}

int	CGadget::getWidth()
{
	RECT winBounds;

	GetWindowRect(myHwnd,&winBounds);

	return (winBounds.right-winBounds.left);
}

int CGadget::getClientWidth()
{
	WINDOWINFO winInfo;
	winInfo.cbSize=sizeof(WINDOWINFO);

	GetWindowInfo(myHwnd,&winInfo);

	return (winInfo.rcClient.right-winInfo.rcClient.left);
}

int CGadget::getHeight()
{
	RECT winBounds;

	GetWindowRect(myHwnd,&winBounds);

	return (winBounds.bottom-winBounds.top);
}

int CGadget::getClientHeight()
{
	WINDOWINFO winInfo;
	winInfo.cbSize=sizeof(WINDOWINFO);

	GetWindowInfo(myHwnd,&winInfo);

	return (winInfo.rcClient.bottom-winInfo.rcClient.top);
}

int CGadget::getX(bool returnAbsolute, bool returnClient)
{
	//This is rather fiddly as I can't see a way to get relative co-ords for windows.

	//Get left co-ord of gadget's window and subtract left co-ord of parent window's client
	//area.

	WINDOWINFO	winBounds;
	WINDOWINFO	parentWinBounds;

	ZeroMemory(&parentWinBounds,sizeof(WINDOWINFO)); //Prevents rubbish being returned for top-level windows
	ZeroMemory(&winBounds,sizeof(WINDOWINFO));

	winBounds.cbSize=sizeof(WINDOWINFO);
	parentWinBounds.cbSize=sizeof(WINDOWINFO);

	GetWindowInfo(GetParent(myHwnd),&parentWinBounds);
	GetWindowInfo(myHwnd,&winBounds);


	//GetWindowRect(myHwnd,&winBounds);

	//Does the user want the position relative to the screen (cursor) or relative to the top-left edge of the parent
	//window?

	if (returnAbsolute)
	{
		if (returnClient)
		{
			return winBounds.rcClient.left;
		}
		else
		{
			return winBounds.rcWindow.left;	
		}
	}
	else
	{
		if (returnClient)
		{
			return winBounds.rcClient.left-parentWinBounds.rcClient.left;
		}
		else
		{
			return winBounds.rcWindow.left-parentWinBounds.rcClient.left;
		}
	}
}

int CGadget::getY(bool returnAbsolute, bool returnClient)
{
	//This is rather fiddly as I can't see a way to get relative co-ords for windows.

	//Get top co-ord of gadget's window and subtract top co-ord of parent window's client
	//area.

	WINDOWINFO	winBounds;
	WINDOWINFO	parentWinBounds;

	ZeroMemory(&parentWinBounds,sizeof(WINDOWINFO));
	ZeroMemory(&winBounds,sizeof(WINDOWINFO));

	winBounds.cbSize=sizeof(WINDOWINFO);
	parentWinBounds.cbSize=sizeof(WINDOWINFO);

	GetWindowInfo(GetParent(myHwnd),&parentWinBounds);
	GetWindowInfo(myHwnd,&winBounds);
	
	//Does the user want the position relative to the screen (cursor) or relative to the top-left edge of the parent
	//window?
	if (returnAbsolute)
	{
		if (returnClient)
		{
			return winBounds.rcClient.top;
		}
		else
		{
			return winBounds.rcWindow.top;
		}
	}
	else
	{
		if (returnClient)
		{
			return winBounds.rcClient.top-parentWinBounds.rcClient.top;
		}
		else
		{
			return winBounds.rcWindow.top-parentWinBounds.rcClient.top;
		}
	}
}

void CGadget::setText(char* newCaption)
{
	//Send message to window telling it to update
	SendMessage(myHwnd,WM_SETTEXT,NULL,(LPARAM)newCaption);
}

void CGadget::alignText(TEXT_ALIGN textAlignment)
{
	//Base implementation does nothing
}

void CGadget::handleNotification(LPNMHDR event)
{

}

void CGadget::sendNotification(int eventCode)
{
	NMHDR event;

	event.hwndFrom=myHwnd;
	event.code=eventCode;

	SendMessage(myHwnd,WM_NOTIFY,0,(LPARAM)&event);
}

void CGadget::setTabFocus(bool tabFocus)
{
	if (tabFocus)
	{
		SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|WS_TABSTOP);
	}
	else
	{
		SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)&(~WS_TABSTOP));
	}
}

char* CGadget::getText()
{
	//Add one on the end for the null character terminating the string
	int maxLen=GetWindowTextLength(myHwnd)+1;

	//MEMORY
	//TODO - This buffer should really be allocated by the calling function to prevent memory leaks
	char* winText=new char[maxLen];

	GetWindowText(myHwnd,winText,maxLen);

	return winText;

}

bool CGadget::getEvents(bool waitForEvent)
{
	
	
	//Exit if events object has not yet been initialised
	if (!events)
	{
		return false;
	}

	//Clear events if they do exist
	if (events)
	{
		CGadget::events->clearEvent();
	}

     
	MSG		messageInfo;
	BOOL	eventResult=FALSE;

	//If waitForEvent is true, the program does not continue execution until an event has been received
	if (waitForEvent)
	{
		eventResult=GetMessage(&messageInfo,NULL,NULL,NULL);  // This will always return true
	}
	else
	{
		eventResult=PeekMessage(&messageInfo,NULL,NULL,NULL,PM_REMOVE); //This may return true or false
	}

	if (eventResult)
	{
		//Get source gadget
		CGadget* src=convertWinHandle(messageInfo.hwnd);

		//If the source of the message is a gadget, then set the events class, so that we can use the eventSource(),eventType() etc. functions
		//to get information about the events
		if (src != NULL)
		{
			CGadget::events->setEvent((DWORD)src,messageInfo.message,messageInfo.wParam,messageInfo.lParam);
		}

		TranslateMessage(&messageInfo);
		DispatchMessage(&messageInfo);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadget::drawPoint(DWORD x, DWORD y,COLORREF color)
{
	//Sets the colour of the specified pixel on the current gadget

	//Get a device context for drawing into
	HDC drawSurf=GetDC(myHwnd);

	//SetPixelV is slightly faster than the alternative SetPixel because it doesn't return any values
	//note that if the display mode does not support the specified colour, SetPixel will use a near match instead
	SetPixelV(drawSurf,x,y,color);

	//Tidyup
	ReleaseDC(myHwnd,drawSurf);
}

LRESULT CGadget::sendMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	//Wrap SendMessage
	return SendMessage(myHwnd,message,wParam,lParam);
}

bool CGadget::loadImageFromFile(char* fileName)
{
	//Note:  Currently transparency is enabled
	//Transparency only works on 256-colour images.
	//LR_LOADMAP3DCOLORS ensures that transparent areas are replaced with grey rather than white.
	gadgetImage = (HBITMAP)LoadImage(NULL,fileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_LOADMAP3DCOLORS|LR_LOADTRANSPARENT);

	if (gadgetImage)
	{
		//Call applyImage method for objects inherited from CGadget to actually make the image
		//appear on the gadget

		applyImage();

		return true;
	}
	else
	{
		return false;
	}
}

bool CGadget::loadImageFromMem(int imageIndex)
{
	//Get DBPro core pointer so we can get memblocks & images function pointers.
#ifdef DBPRO_DLL
	core = CApplication::core;
#endif

	PFMakeMemblockFromImage makeMemblockFromImage	= CApplication::makeMemblockFromImage;
	PFMemblockExists		memblockExists			= CApplication::memblockExists;
	PFGetMemblockPtr		getMemblockPtr			= CApplication::getMemblockPtr;
	PFGetMemblockSize		getMemblockSize			= CApplication::getMemblockSize;
	PFDeleteMemblock		deleteMemblock			= CApplication::deleteMemblock;
	PFImageExists			imageExists				= CApplication::imageExists;

	//Exit function if any of the functions were not found in the memblocks / image DLLs	
	if 
	(
		(!makeMemblockFromImage)	|| 
		(!memblockExists)			|| 
		(!getMemblockPtr)			||
		(!getMemblockSize)			||
		(!deleteMemblock)			||
		(!imageExists)
	)
	{
		return false;
	}
	
	//Function pointers valid - do clever stuff :)

	//First check that the image exists
	if (!imageExists(imageIndex))
	{
		return false;
	}

	//Then find a free memblock ID
	DWORD memblockId=1;

	while (memblockExists(memblockId))
	{
		memblockId++;
	}
	//DBPro only allows up to 255 memblocks
	if (memblockId > 255) 
	{
		return false;
	}

	//Create a memblock from the image
	makeMemblockFromImage(memblockId,imageIndex);

	if (!memblockExists(memblockId))
	{
		return false;
	}

	//Get a pointer to the memblock
	DWORD* memblockPtr=(DWORD*)getMemblockPtr(memblockId);

	if (!memblockPtr)
	{
		return false;
	}

	//Get the size, width & depth of the image
	//Note memblock structure:  WIDTH [4 Bytes], HEIGHT [4 Bytes], DEPTH [4 Bytes], PIXEL DATA [Width*Height*(Depth/8) Bytes]
	DWORD imageWidth	= *(memblockPtr);
	DWORD imageHeight	= *(memblockPtr+1);
	DWORD imageDepth	= *(memblockPtr+2);
	DWORD pixelDataSize	= imageWidth*imageHeight*(imageDepth/8);
	DWORD offset		= 3; //Set offset to first DWORD past 3-DWORD header

	//Obtain a handle to the gadget's device context
	HDC windowDC		= GetDC(myHwnd);

	//Create a compatible memory DC for faster operation
	
	//Only create the bitmap & DC if we have not already created one, otherwise we end up with a nasty, evil memory
	//leak.  
	if (gadgetImage == 0)
	{
		gadgetImage	= CreateCompatibleBitmap(windowDC,imageWidth,imageHeight);
	}
	HBITMAP bitmap = gadgetImage;

	if (gadgetDC == 0)
	{
		gadgetDC	= CreateCompatibleDC(windowDC); 
		RealizePalette(gadgetDC);
	}
	HDC compatibleDC=gadgetDC;
	//Select the bitmap into the memory DC
	HGDIOBJ originalBitmap=SelectObject(gadgetDC,bitmap);	
	
	
	ReleaseDC(myHwnd,windowDC); //We get errors if we don't release this first (why?)

	int gdiRedValue			=0;
	int gdiGreenValue		=0;
	int gdiBlueValue		=0;
	DWORD dbproColorValue	=0;

	//Copy the data pixel-by-pixel
	for (DWORD y=0;y<imageHeight;y++)
	{
		for (DWORD x=0;x<imageWidth;x++)
		{
			//Retrieve next pixel's colour value
			dbproColorValue=*(memblockPtr+offset);

			//Note:  DBPro pixel colour is 0x00RRGGBB, GDI is 0x00BBGGRR, so we have to convert
			gdiBlueValue=GetRValue((COLORREF)dbproColorValue);
			gdiGreenValue=GetGValue((COLORREF)dbproColorValue);
			gdiRedValue=GetBValue((COLORREF)dbproColorValue);

			//Write pixel to DC (and therefore bitmap)
			SetPixelV(compatibleDC,x,y,RGB(gdiRedValue,gdiGreenValue,gdiBlueValue));
			//Increment offset to we read from next pixel
			offset++;
		}
	}

	//Select our new bitmap out of the DC so we can delete the Device Context
	SelectObject(compatibleDC,originalBitmap);

	//Set the member gadgetImage to our new bitmap
	gadgetImage=bitmap;

	deleteMemblock(memblockId);
	//Phew! managed all that, let's return true!
	return true;
}

//Send a timer message to the window after the specified amount of time
void CGadget::setTimer(DWORD milliseconds)
{
	if (milliseconds==0)
	{
		KillTimer(myHwnd,1); //If milliseconds is 0, timer is disabled
	}
	else
	{
		SetTimer(myHwnd,1,milliseconds,0); //Always timer #1, multiple timers per gadget not supported
	}
}

//Repaint gadget window
void CGadget::update()
{
	//Temporarily enable auto-update
	
	bool updateSetting=getAutoUpdate();

	//Set to true to force gadget to repaint itself
	setAutoUpdate(true);
	
	//sendMsg(WM_PAINT,0,0);
	RedrawWindow(myHwnd,0,0,RDW_INVALIDATE);

	//Restore original setting
	setAutoUpdate(updateSetting);
}

bool CGadget::wantTabs()
{
	return false;
}

void CGadget::enableXPStyle(bool xpStyle)
{
	typedef HRESULT (__stdcall *PFAPISetWindowTheme) (HWND,LPCWSTR,LPCWSTR);
	PFAPISetWindowTheme APISetWindowTheme;

	APISetWindowTheme=(PFAPISetWindowTheme)GetProcAddress(GetModuleHandle("uxtheme.dll"),"SetWindowTheme");

	if (!APISetWindowTheme)
	{
		return;
	}

	if (xpStyle)
	{
		APISetWindowTheme(myHwnd,0,0);
	}
	else
	{
		APISetWindowTheme(myHwnd,L" ",L" ");
	}
		
}


//Default implementation of applyImage
void CGadget::applyImage()
{
}

//Sets a gadget's parent window
void CGadget::setParent(HWND parentWindow)
{
	SetParent(myHwnd,parentWindow);
}

//Align the gadget according to its auto-align property
void CGadget::doAlign()
{
	//Get information about parent window
	HWND parentWindow=GetParent(myHwnd);

	//Use the desktop window as a reference if the window doesn't have a parent
	if (!parentWindow)
	{
		parentWindow=GetDesktopWindow();
	}

	if (parentWindow)
	{
		WINDOWINFO	parentWinBounds;

		ZeroMemory(&parentWinBounds,sizeof(WINDOWINFO)); //Prevents rubbish being returned for top-level windows
		parentWinBounds.cbSize=sizeof(WINDOWINFO);

		GetWindowInfo(parentWindow,&parentWinBounds);
			
		//Calculate the width and height of the parent window's client area
		int	parentHeight=0;
		int parentWidth=0;

		//Use a different method if we are dealing with the desktop window as that doesn't have a client area
		if (parentWindow != GetDesktopWindow())
		{
			parentWidth=parentWinBounds.rcClient.right-parentWinBounds.rcClient.left;
			parentHeight=parentWinBounds.rcClient.bottom-parentWinBounds.rcClient.top;
		}
		else
		{
			parentWidth=parentWinBounds.rcWindow.right-parentWinBounds.rcWindow.left;
			//Subtract 20 pixels to account for start menu height
			//TODO - A better way of managing this
		//	WINDOWINFO startButton;
		//	GetWindowInfo(FindWindow(0,"start"),&startButton);
		//	DWORD startButtonHeight=startButton.rcWindow.bottom-startButton.rcWindow.top;
		//	debugInt("startbtn",(int)FindWindow(0,"start"));
		//	debugInt("height",(int)startButtonHeight);
			parentHeight=parentWinBounds.rcWindow.bottom-parentWinBounds.rcWindow.top-32;
		}

		//Resize/move the gadget depending on the align parameter
		switch (align)
		{
		case ALIGN_ALL:
			move(0,0,parentWidth,parentHeight); //Fills width & height of parent
			break;
		case ALIGN_TOP:
			move(0,0,parentWidth,getHeight()); //Fills top of parent
			break;
		case ALIGN_BOTTOM:
			move(0,parentHeight-getHeight(),parentWidth,getHeight()); //Fills bottom of parent
			break;
		case ALIGN_LEFT:
			move(0,0,getWidth(),parentHeight); //Fills left of parent
			break;
		case ALIGN_RIGHT:
			move(parentWidth-getWidth(),0,getWidth(),parentHeight); //Fills right of parent
			break;
		default:
			break;
		}
	}
}

void CGadget::setEnabled(DWORD flag)
{
	EnableWindow(myHwnd,(BOOL)flag);
}

DWORD CGadget::getEnabled()
{
	return (DWORD)IsWindowEnabled(myHwnd);
}

void CGadget::makeActive()
{
	SetFocus(myHwnd);
}

//TODO - Not yet finished
void CGadget::doDragResize()
{
	//-Check that resizing is enabled
	//-Calculate relative mouse coordinates for the gadget
	//-Check that mouse is inside drag region - set cursor to sizing if true
	//-Check that mouse is down
	//-Resize as necessary

	//Exit if drag resizing is disabled
	if (myDragResizeSide==0)
	{
		return;
	}
	//Get the cursor position and calculate its position relative to the gadget
	POINT cursorPos;

	GetCursorPos(&cursorPos);

	int			relMouseX	=cursorPos.x-getX(true);
	int			relMouseY	=cursorPos.y-getY(true);
	bool		canDrag		=false;
	bool		sizing		=false;

	//Set relative positions to -1 if cursor is outside window
	if ((relMouseY < 0) || (relMouseY > getHeight()))
	{
		relMouseY=-1;
		relMouseX=-1;
	}

	if ((relMouseX < 0) || (relMouseX > getWidth()))
	{
		relMouseX=-1;
		relMouseY=-1;
	}

	//Determine whether the cursor is in the correct region of the gadget to allow drag-resizing
	int isCursorInCorrectPositionForResize=0;

	if (myDragResizeSide==1)
	{
		isCursorInCorrectPositionForResize=relMouseX > (getWidth()-10);
	}
	if (myDragResizeSide==2)
	{
		isCursorInCorrectPositionForResize=((relMouseX > 0) && (relMouseX < -10));
	}


	if (isCursorInCorrectPositionForResize)
	{
		SetCursor(CApplication::sizingCursor);
		canDrag=true;
	}
	else
	{
		canDrag=false;
	}

	if ((myMouseDown==true) && (isCursorInCorrectPositionForResize))
	{
		sizing=true;
	}

	if (myMouseDown==false)
	{
		sizing=false;
	}

	//debugInt("canDrag",(int)canDrag);
	//debugInt("sizing",(int)sizing);

	if (sizing)
	{
		if (myDragResizeSide==1)
		{
			if (getWidth() != relMouseX)
			{
				move(getX(),getY(),relMouseX,getHeight());
			}
		}

		
	}

	SetCursor(CApplication::sizingCursor);
}

//CGadgetButton
//================================================================================================
bool CGadgetButton::create(int x, int y, int width, int height, char* caption, bool flat, HWND parent)
{
	DWORD extraStyles=0;

	if (flat)
	{
		extraStyles=extraStyles|BS_FLAT;
	}

	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindow("BUTTON",caption,defaultStyles|extraStyles|BS_NOTIFY,x,y,width,height,parent,0,NULL,0);
	
	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		
		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetButton::handleNotification(LPNMHDR event)
{
	if (event->code==BN_SETFOCUS)
	{
		_setBoldFocus(true);

		/*if ((BoldFocusButton) && (BoldFocusButton != this))
		{
			BoldFocusButton->_setBoldFocus(false);
		}

		BoldFocusButton=this;*/
	}

	if (event->code==BN_KILLFOCUS)
	{
		_setBoldFocus(false);
	}
}

void CGadgetButton::_setBoldFocus(bool boldFocus)
{
	if (!boldFocus)
	{
		SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE) - BS_DEFPUSHBUTTON);
		this->update();
	}
	else
	{
		SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|BS_DEFPUSHBUTTON);
	}
}

void CGadgetButton::applyImage()
{
	//We need to turn the button into an image button by adding the BS_BITMAP style
	SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|BS_BITMAP);
	//Tell gadget to load its bitmap
	sendMsg(BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)gadgetImage);
	DWORD a=0;
	a=(DWORD)sendMsg(BM_GETIMAGE,IMAGE_BITMAP,0);
}

//CGadgetFrame
//================================================================================================
bool CGadgetFrame::create(int x, int y, int width, int height, char* caption, HWND parent)
{
	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
		
	//Frames are basically just buttons but with the BS_GROUPBOX style, they look like panels but with a captioned
	//single-line border around the edge.  

	myHwnd=CreateWindowEx(0,"BUTTON",caption,defaultStyles|BS_NOTIFY|BS_GROUPBOX,x,y,width,height,parent,0,NULL,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		setManualBackgroundPaint(true);

		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		return true;
	}
	else
	{
		return false;
	}
}

//CGadgetCheckbox
//================================================================================================

bool CGadgetCheckbox::create(int x, int y, int width, int height, char* caption, HWND parent)
{
	//Checkbox gadgets are basically just buttons with the BS_AUTOCHECKBOX style
	myHwnd=CreateWindow("BUTTON",caption,defaultStyles|BS_AUTOCHECKBOX,x,y,width,height,parent,0,NULL,0);

	//Was window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it

		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_CHECKBOX);

		return true;
	}
	else
	{
		return false;
	}
}

bool CGadgetCheckbox::getChecked()
{
	if (sendMsg(BM_GETCHECK,0,0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetCheckbox::setChecked(bool checked)
{
	if (checked)
	{
		sendMsg(BM_SETCHECK,BST_CHECKED,0);
	}
	else
	{
		sendMsg(BM_SETCHECK,BST_UNCHECKED,0);
	}
}

//CGadgetRadiobutton
//================================================================================================

bool CGadgetRadiobutton::create(int x, int y, int width, int height, char* caption, HWND parent)
{
	//Checkbox gadgets are basically just buttons with the BS_AUTOCHECKBOX style
	myHwnd=CreateWindow("BUTTON",caption,defaultStyles|BS_AUTORADIOBUTTON,x,y,width,height,parent,0,NULL,0);

	//Was window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_RADIOBUTTON);

	//	setManualBackgroundPaint(true);

		return true;
	}
	else
	{
		return false;
	}
}

bool CGadgetRadiobutton::getChecked()
{
	if (sendMsg(BM_GETCHECK,0,0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetRadiobutton::setChecked(bool checked)
{
	
	if (checked)
	{
		sendMsg(BM_SETCHECK,BST_CHECKED,0);

		//Clear other sibling radio buttons
		HWND nextWindow=GetWindow(myHwnd,GW_HWNDFIRST);

		while (nextWindow)
		{
			if (nextWindow != myHwnd)
			{
				//Make sure that the control is a gadget and that it is a radio button before we uncheck it.
				CGadget* sibling=(CGadget*)(GetWindowLong(nextWindow,GWL_USERDATA));
				
				if (sibling)
				{
					if (sibling->getType()==GADGET_RADIOBUTTON)
					{
						SendMessage(nextWindow,BM_SETCHECK,BST_UNCHECKED,0);
					}
				}
			}

			nextWindow=GetWindow(nextWindow,GW_HWNDNEXT);
		}
	}
	else
	{
		sendMsg(BM_SETCHECK,BST_UNCHECKED,0);
	}
}


//CGadgetEdit
//================================================================================================

bool CGadgetEdit::create(int x, int y, int width, int height, int settings,HWND parent)
{
	//Initialize internal properties
	isReadOnly=false;

	//Note: Create with WS_EX_CLIENTEDGE style to get nice-looking sunken border
	//Use ES_AUTOHSCROLL to ensure that text in box is automatically scrolled in the horizontal direction

	//Add ES_MULTILINE style if creating a multi-line textarea style gadget, also add ES_AUTOVSCROLL to ensure that
	//user can scroll text inside the edit gadget

	DWORD additionalStyles=0;

	bool multiLine=(settings & 1);
	isMultiLine=multiLine;
	
	if (settings & 2)
	{
		additionalStyles += ES_NUMBER;
	}

	if (settings & 4)
	{
		additionalStyles |= ES_AUTOHSCROLL|WS_HSCROLL;
	}


	if (multiLine)
	{
		myHwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",NULL,defaultStyles|ES_AUTOVSCROLL|ES_MULTILINE|WS_VSCROLL
			|ES_WANTRETURN|ES_LEFT|additionalStyles,x,y,width,height,parent,NULL,NULL,0);
	}
	else
	{
		//ES_AUTOHSCROLL must only be used for single line edit boxes.  It ensures that the box scrolls if the user moves the cursor
		//to the right to see more text
		myHwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",defaultStyles|ES_AUTOHSCROLL|additionalStyles
			,x,y,width,height,parent,0,NULL,0);
	}

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_EDIT);

		return true;
	}
	else
	{
		return false;
	}

	
}

void CGadgetEdit::alignText(TEXT_ALIGN textAlignment)
{
	

	DWORD styles=GetWindowLong(myHwnd,GWL_STYLE);

	//Turn off existing styles
	styles &= ~ES_LEFT;
	styles &= ~ES_RIGHT;
	styles &= ~ES_CENTER;

	//Apply new style
	switch (textAlignment)
	{
	case TEXT_ALIGN_LEFT:
		styles |= ES_LEFT;
		break;
	case TEXT_ALIGN_RIGHT:
		styles |= ES_RIGHT;
		break;
	case TEXT_ALIGN_CENTER:
		styles |= ES_CENTER;
	
		break;
	}

	//Change window
	SetWindowLong(myHwnd,GWL_STYLE,styles);
}

int	CGadgetEdit::getSelStart()
{
	//LOWORD stores start pos, HIWORD stores end pos
	DWORD selInfo=SendMessage(myHwnd,EM_GETSEL,0,0);

	return LOWORD(selInfo);
}

int CGadgetEdit::getSelLen()
{
	//LOWORD stores start pos, HIWORD stores end pos
	DWORD selInfo=SendMessage(myHwnd,EM_GETSEL,0,0);

	return HIWORD(selInfo)-LOWORD(selInfo);
}

void CGadgetEdit::setSel(int start,int len)
{
	//start parameter is zero-based index of first char to be selected.

	//NOTE:  MSDN documentation regarding EM_SETSEL is incorrect at time of coding,
	//it says lParam is used to set start/end using SendMessage(win,EM_SETSEL,0,MAKELONG(start,end));
	//which doesn't work.
	SendMessage(myHwnd,EM_SETSEL,start,start+len);
}

//Multi-line edits swallow tabs, single line edits do not.
bool CGadgetEdit::wantTabs()
{
	return isMultiLine;
}

//Specifies whether or not a gadget's text can be edited
//Read only edits have a grey background to signfy that they are locked
void CGadgetEdit::setReadOnly(bool readOnly)
{
	if (readOnly)
	{
		SendMessage(myHwnd,EM_SETREADONLY,TRUE,0);
	}
	else
	{
		SendMessage(myHwnd,EM_SETREADONLY,FALSE,0);
	}
	
	//Set internal read-only property
	isReadOnly=readOnly;
}

void CGadgetEdit::setSelText(char* text)
{
	SendMessage(myHwnd,EM_REPLACESEL,0,(LPARAM)text);
}

void CGadgetEdit::setMaxTextLength(int length)
{
	sendMsg(EM_LIMITTEXT,length,0);
}


bool CGadgetEdit::getReadOnly()
{
	//Return internal property
	return isReadOnly;
}

//CGadgetScrollbar
//================================================================================================
bool CGadgetScrollbar::create(int x, int y, int width, int height, int style, HWND parent)
{
	//0=Horizontal, 1=Vertical
	if (style==0)
	{
		myHwnd=CreateWindow("SCROLLBAR",0,defaultStyles|SBS_HORZ,x,y,width,height,parent,0,0,0);
	}
	else
	{
		myHwnd=CreateWindow("SCROLLBAR",0,defaultStyles|SBS_VERT,x,y,width,height,parent,0,0,0);
	}

	//Was the window created successfully?
	if (myHwnd)
	{
		myGadgetType=GADGET_SCROLLBAR;

		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		//Set default scrollbar range from 0 to 100
		sendMsg(SBM_SETRANGEREDRAW,0,100);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetScrollbar::setRange(int min, int max)
{
	sendMsg(SBM_SETRANGE,min,max);
}

void CGadgetScrollbar::setPosition(int position)
{
	sendMsg(SBM_SETPOS,position,TRUE);
}

int CGadgetScrollbar::getMinimumValue()
{
	int value=0;
	sendMsg(SBM_GETRANGE,(WPARAM)&value,0);
	return value;
}

int CGadgetScrollbar::getMaximumValue()
{
	int value=0;
	sendMsg(SBM_GETRANGE,0,(LPARAM)&value);
	return value;
}

int CGadgetScrollbar::getPosition()
{
	return sendMsg(SBM_GETPOS,0,0);
}

void CGadgetScrollbar::setThumbSize(int size)
{
	SCROLLINFO scrollInfo;

	scrollInfo.cbSize=sizeof(SCROLLINFO);
	scrollInfo.fMask=SIF_PAGE;
	scrollInfo.nPage=size;

	sendMsg(SBM_SETSCROLLINFO,TRUE,(LPARAM)&scrollInfo);
}

int CGadgetScrollbar::getThumbSize()
{
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize=sizeof(SCROLLINFO);
	scrollInfo.fMask=SIF_PAGE;

	sendMsg(SBM_GETSCROLLINFO,0,(LPARAM)&scrollInfo);

	return scrollInfo.nPage;
}

//CGadgetStatic
//================================================================================================
      
bool CGadgetStatic::create(int x, int y, int width, int height, char* caption, HWND parent)
{
	//WS_EX_ACCEPTFILES
	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created

	myHwnd=CreateWindowEx(NULL,"STATIC",caption,defaultStyles|SS_NOTIFY,x,y,width,height,parent,0,NULL,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetStatic::setBorderStyle(PANEL_BORDER_STYLE borderStyle)
{
	//TIDYUP - Not v. efficient to set window flags like this

	//Adds or removes bit flags to change border style
	switch (borderStyle)
	{
	case PB_NONE:
		SetWindowLong(myHwnd,GWL_EXSTYLE,NULL);
		SetWindowLong(myHwnd,GWL_STYLE,defaultStyles|SS_NOTIFY);
		break;
	case PB_3D_RAISED:
		SetWindowLong(myHwnd,GWL_EXSTYLE,NULL);
		SetWindowLong(myHwnd,GWL_STYLE,defaultStyles|SS_NOTIFY|WS_DLGFRAME);
		break;
	case PB_3D_SUNKEN:
		SetWindowLong(myHwnd,GWL_STYLE,defaultStyles|SS_NOTIFY);
		SetWindowLong(myHwnd,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
		break;
	case PB_FLATBLACK:
		SetWindowLong(myHwnd,GWL_EXSTYLE,NULL);
		SetWindowLong(myHwnd,GWL_STYLE,defaultStyles|WS_BORDER|SS_NOTIFY);
		break;
	}

	//Call SetWindowPos to force the window to repaint itself.  The SWP_FRAMECHANGED flag tells the window
	//that it needs to redraw the frames (whoose images are otherwise cached)
	SetWindowPos(myHwnd,NULL,getX(),getY(),getWidth(),getHeight(),SWP_FRAMECHANGED);
}

void CGadgetStatic::setTransparent(bool transparent)
{
	isTransparent=transparent;

	if (transparent)
	{
		SetWindowLong(myHwnd,GWL_EXSTYLE,GetWindowLong(myHwnd,GWL_EXSTYLE)| WS_EX_TRANSPARENT);
	}
	else
	{
		SetWindowLong(myHwnd,GWL_EXSTYLE,GetWindowLong(myHwnd,GWL_EXSTYLE) &~ WS_EX_TRANSPARENT);
	}
}

void CGadgetStatic::setText(char* newCaption)
{
	if (isTransparent)
	{
		HWND parent=GetParent(myHwnd);

		RECT rect;
		POINT ptTopLeft;
		POINT ptBottomRight;

		GetWindowRect(myHwnd,&rect);
		ptTopLeft.x=rect.left;
		ptTopLeft.y=rect.top;
		ptBottomRight.x=rect.right;
		ptBottomRight.y=rect.bottom;

		ScreenToClient(parent,&ptTopLeft);
		ScreenToClient(parent,&ptBottomRight);

		rect.left=ptTopLeft.x;
		rect.right=ptBottomRight.x;
		rect.top=ptTopLeft.y;
		rect.bottom=ptBottomRight.y;

		//sendMsg(WM_SETTEXT,(WPARAM)parent,(LPARAM)"Wo");

		InvalidateRect(parent,&rect,TRUE);
		UpdateWindow(parent);
	}

	CGadget::setText(newCaption);
}

DWORD CGadgetStatic::customPaint(HDC deviceContext)
{
	if (isTransparent)
	{	
		SetBkMode(deviceContext,TRANSPARENT);
		
		return (DWORD)GetStockObject(NULL_BRUSH);
	}
	else
	{
		return CGadget::customPaint(deviceContext);
	}
}

void CGadgetStatic::applyImage()
{
	//We need to turn the static control into an image button by adding the BS_BITMAP style
	SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|SS_BITMAP);
	//Tell gadget to load its bitmap
	sendMsg(STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)gadgetImage);
	
	//DWORD a;
	//STM_GETIMAGE
	//a=(DWORD)sendMsg(BM_GETIMAGE,IMAGE_BITMAP,0);
}

void CGadgetStatic::alignText(TEXT_ALIGN textAlignment)
{
		DWORD styles=GetWindowLong(myHwnd,GWL_STYLE);

	//Turn off existing styles
	styles &= ~SS_LEFT;
	styles &= ~SS_RIGHT;
	styles &= ~SS_CENTER;

	//Apply new style
	switch (textAlignment)
	{
	case TEXT_ALIGN_LEFT:
		styles |= SS_LEFT;
		break;
	case TEXT_ALIGN_RIGHT:
		styles |= SS_RIGHT;
		break;
	case TEXT_ALIGN_CENTER:
		styles |= SS_CENTER;
	
		break;
	}

	debugInt("Aligning text",0);

	//Change window
	SetWindowLong(myHwnd,GWL_STYLE,styles);
}

//CGadgetListbox
//================================================================================================

bool CGadgetListbox::create(int x, int y, int width, int height, HWND parent)
{
	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"LISTBOX",NULL,defaultStyles|LBS_HASSTRINGS|WS_VSCROLL,x,y,width,height,parent,0,NULL,0);
	
	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_LISTBOX);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetListbox::addItem(char* itemText, int index)
{
	SendMessage(myHwnd,LB_INSERTSTRING,index,(LPARAM)itemText);
}

int CGadgetListbox::selectedItem()
{
	return SendMessage(myHwnd,LB_GETCURSEL,0,0);
}

char* CGadgetListbox::getItemText(int index)
{
	if (index==-1)
	{
		return NULL;
	}

	//First get necessary buffer length
	DWORD itemLen=SendMessage(myHwnd,LB_GETTEXTLEN,index,NULL);

	//MEMORY - New pointer allocated which must be deleted by user.
	//TODO - We should get the user to allocate the memory
	if (itemLen != 0)
	{
		//Create buffer of correct length and get item text into buffer
		char* itemText=new char[itemLen+1];
		
		//Get Text
		SendMessage(myHwnd,LB_GETTEXT,index,(LPARAM)itemText);

		//Return pointer to buffer
		return itemText;
	}
	else
	{
		//Create and return a null buffer
		char* itemText=new char[1];
		*itemText=0;

		return itemText;
	}

	return NULL;
}

void CGadgetListbox::removeItem(int index)
{
	//Do not attempt to remove the item if no items are selected (ie index==-1)
	if (index==-1)
	{
		return;
	}

	SendMessage(myHwnd,LB_DELETESTRING,index,0);

}

void CGadgetListbox::changeItem(int index, char* newCaption)
{
	bool isSelected=(selectedItem()==index);


	//Remove the existing item then insert a new one in the same position

	if (SendMessage(myHwnd,LB_DELETESTRING,index,0) != LB_ERR)
	{
		//Only insert item if removal was successful
		SendMessage(myHwnd,LB_INSERTSTRING,index,(LPARAM)newCaption);

		//If we modified the selected item, re-select the new item
		if (isSelected)
		{
			selectItem(index);
		}
	}

}

void CGadgetListbox::clear()
{
	SendMessage(myHwnd,LB_RESETCONTENT,0,0);
}

DWORD CGadgetListbox::getItemCount()
{
	return SendMessage(myHwnd,LB_GETCOUNT,0,0);
}

//Sets the first item which is visible in the listbox (ie. all items above it can only be seen by scrolling the box.
//useful in chat applications for example where we want to scroll the box after each message is posted)
void CGadgetListbox::setTopIndex(int index)
{
	SendMessage(myHwnd,LB_SETTOPINDEX,index,0);
}

void CGadgetListbox::selectItem(int index)
{
	SendMessage(myHwnd,LB_SETCURSEL,index,0);
}


//CGadgetCombobox
//================================================================================================

bool CGadgetCombobox::create(int x, int y, int width, int height, HWND parent)
{
	//Set a minimum height to ensure that the drop down list is always visible
	if (height < 100)
	{
		height=100;
	}

	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindowEx(NULL,"COMBOBOX",NULL,defaultStyles|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS
		,x,y,width,height,parent,0,NULL,0);
	
	//Set default properties

	currentSel=0;

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_COMBOBOX);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetCombobox::addItem(char* itemText, int index)
{
	sendMsg(CB_INSERTSTRING,index,(LPARAM)itemText);
}

void CGadgetCombobox::removeItem(int index)
{
	sendMsg(CB_DELETESTRING,index,0);
}

void CGadgetCombobox::changeItem(int index, char* newCaption)
{
	bool isSelected=(selectedItem() == index);
	
	//First remove item then add new one
	if (sendMsg(CB_DELETESTRING,index,0) != CB_ERR)
	{
		sendMsg(CB_INSERTSTRING,index,(LPARAM)newCaption);

		//If we have just removed and re-inserted the selected item, then re-select the item
		if (isSelected)
		{
			selectItem(index);
		}
	}
}

void CGadgetCombobox::clear()
{
	sendMsg(CB_RESETCONTENT,0,0);
}

void CGadgetCombobox::selectItem(int index)
{
	sendMsg(CB_SETCURSEL,index,0);
}

int CGadgetCombobox::selectedItem()
{
	//Selected item should only be updated when the drop down list is closed.

	if (sendMsg(CB_GETDROPPEDSTATE,0,0))
	{
		return currentSel;
	}
	else
	{
		//Possible change - update combobox
		currentSel=sendMsg(CB_GETCURSEL,0,0);

		return currentSel;
	}
}

char* CGadgetCombobox::getItemText(int index)
{
	if (index==-1)
	{
		return NULL;
	}

	//First get necessary buffer length
	DWORD itemLen=SendMessage(myHwnd,CB_GETLBTEXTLEN,index,NULL);

	//MEMORY - New pointer allocated which must be deleted by user.
	if (itemLen != 0)
	{
		//Create buffer of correct length and get item text into buffer
		char* itemText=new char[itemLen+1];
		SendMessage(myHwnd,CB_GETLBTEXT,index,(LPARAM)itemText);

		//Return pointer to buffer
		return itemText;
	}
	else
	{
		//Create and return a null buffer
		char* itemText=new char[1];
		*itemText=0;

		return itemText;
	}

	return NULL;
}

DWORD CGadgetCombobox::getItemCount()
{
	return sendMsg(CB_GETCOUNT,0,0);
}

    
//CGadgetTabs
//================================================================================================

bool CGadgetTabs::create(int x, int y, int width, int height, int style, HWND parent)
{
	int additionalStyles=0;

	if (style & 1)
	{
		additionalStyles |= TCS_MULTILINE;
	}

	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindowEx(NULL,WC_TABCONTROL,NULL,defaultStyles|additionalStyles
		,x,y,width,height,parent,0,NULL,0);

	//Was the window created successfully?
	if (myHwnd)
	{

		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);


		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetTabs::handleNotification(LPNMHDR event)
{
	switch (event->code)
	{
		case TCN_SELCHANGE:
			std::list<TabAssociation>::iterator iter;

			int tab=TabCtrl_GetCurSel(myHwnd);

			for (iter=_attachedGadgets.begin();iter != _attachedGadgets.end();++iter)
			{
				TabAssociation assoc=*iter;

				if (assoc.tab==tab)
				{
					assoc.gadget->setVisible(TRUE);
				}
				else
				{
					assoc.gadget->setVisible(FALSE);
				}
			}
		break;
	}
}

void CGadgetTabs::addTab(char* caption, int index)
{
	//Get the number of tabs in the control currently so we can add the new tab onto the end
	DWORD tabCount=sendMsg(TCM_GETITEMCOUNT,0,0);

	//Fill structure with info about new tab
	TCITEM newTab;
	newTab.mask=TCIF_TEXT; //--Change this if image support is added in future
	newTab.iImage=NULL; //Not using images at the moment
	newTab.lParam=NULL;
	newTab.pszText=caption;

	int insertIndex;

	if (index != -1)
	{
		insertIndex=index;
	}
	else
	{
		insertIndex=tabCount;
	}

	//Create new tab - note that index parameter is zero based so if we pass tabCount
	//as the parameter we will add the new tab on the end
	sendMsg(TCM_INSERTITEM,insertIndex,(LPARAM)&newTab);
}

void CGadgetTabs::removeTab(int index)
{
	sendMsg(TCM_DELETEITEM,index,0);
}

void CGadgetTabs::attachGadget(int tab, CGadget* gadget)
{
	std::list<TabAssociation>::iterator iter;

	for (iter = _attachedGadgets.begin();iter != _attachedGadgets.end();++iter)
	{
		if ( (iter->tab==tab) && (iter->gadget==gadget) )
		{
			return;
		}
	}
	
	TabAssociation newAssoc;
	newAssoc.tab=tab;
	newAssoc.gadget=gadget;

	_attachedGadgets.push_back(newAssoc);

	sendNotification(TCN_SELCHANGE);
}

void CGadgetTabs::detatchGadget(int tab, CGadget* gadget)
{
	std::list<TabAssociation>::iterator iter;

	for (iter = _attachedGadgets.begin();iter != _attachedGadgets.end();++iter)
	{
		if ( (iter->tab==tab) && (iter->gadget==gadget) )
		{
			_attachedGadgets.erase(iter);
			return;
		}
	}
}

void CGadgetTabs::changeTab(int index,char* newCaption)
{
	//Fill structure with info about new tab
	TCITEM newTab;
	newTab.mask=TCIF_TEXT; //--Change this if image support is added in future
	newTab.iImage=NULL; //Not using images at the moment
	newTab.lParam=NULL;
	newTab.pszText=newCaption;

	//Create tab attributes
	sendMsg(TCM_SETITEM,index,(LPARAM)&newTab);
}

void CGadgetTabs::setTab(int index)
{
	//Note that index is zero based
	sendMsg(TCM_SETCURSEL,index,0);
}

int CGadgetTabs::selectedTab()
{
	//Returns -1 if no tab is currently selected
	return sendMsg(TCM_GETCURSEL,0,0);
}

char* CGadgetTabs::getTabCaption(int index)
{
	//Create buffer to hold text - this pointer *must* be deleted after usage by the calling
	//function to avoid a memory leak.
	TCITEM tabProperties;
	char* receivedText=new char[255]; //This should be large enough for any usage

	//Setup pointer to buffer & size of buffer in tab properties structure
	tabProperties.mask=TCIF_TEXT;
	tabProperties.cchTextMax=255;
	tabProperties.lParam=NULL;
	tabProperties.iImage=NULL;
	tabProperties.pszText=receivedText;

	//Get properties of specified tab into tabProperties structure
	sendMsg(TCM_GETITEM,index,(LPARAM)&tabProperties);

	//Return a pointer to the text buffer
	return receivedText;
}

int CGadgetTabs::getTabCount()
{
	return sendMsg(TCM_GETITEMCOUNT,0,0);
}

void CGadgetTabs::getDisplayRect(RECT* displayRect)
{
	sendMsg(TCM_ADJUSTRECT,TRUE,(LPARAM)displayRect);
}

//CGadgetStatusbar
//================================================================================================

bool CGadgetStatusbar::create(HWND parent)
{
	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindowEx(NULL,STATUSCLASSNAME,NULL,defaultStyles
		,0,0,75,25,parent,0,NULL,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Set default properties
		autoSize=0;

		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);


		return true;
	}
	else
	{
		return false;
	}
}

int CGadgetStatusbar::getPanelCount()
{
	//Sending SB_GETPARTS with null parameters returns the statusbar's part count
	return sendMsg(SB_GETPARTS,0,0);
}

void CGadgetStatusbar::addPanel(DWORD width, bool enableAutoSize)
{
	  
	//Get current part count
	DWORD partCount=getPanelCount();

	//Create an array to hold existing part details
	int* existingParts=new int[partCount];
	sendMsg(SB_GETPARTS,partCount,(LPARAM)existingParts);


	//Create a new array to hold new part details
	int* newParts=new int[partCount+1];
	DWORD intSize=1;

	DWORD offset=0;
	

	//Copy array details
	for (DWORD i=0;i<(partCount);i++)
	{
		//Copy array element - TODO look up neater way of doing this
		*(newParts+offset)=*(existingParts+offset);
		offset+=intSize;	
	}

	//Add new item on the end - note that the array contains the rightmost co-ord of each panel, so we need to add the width
	//onto the right coord of the previous item.
	*(newParts+offset)=*(newParts+(offset-1))+width;

	sendMsg(SB_SETPARTS,partCount+1,(LPARAM)newParts);

	//Store auto-size property
		bool* tempAutoSize=new bool[partCount+1];
		//Safety check - make sure that an autoSize array already exists
		if (autoSize)
		{
			for (DWORD k=0;k<partCount;k++)
			{
				//Copy array element
				*(tempAutoSize+k)=*(autoSize+k);
			}
			//Delete old bool array
			delete [] autoSize;
		}
		//Copy property for new panel
		*(tempAutoSize+partCount)=enableAutoSize;

		autoSize=tempAutoSize;

	delete [] newParts;
	delete [] existingParts;
}

void CGadgetStatusbar::setPanelText(int index,char* caption)
{
	//If autoSize is enabled, get text dimensions

	//Safety - check that pointer is valid
	if (autoSize)
	{
		//Safety check - ensure that index is valid
		int panelCount=getPanelCount();
		if (index < panelCount)
		{
			
			//Check whether or not autoSize is enabled
			if (*(autoSize+index)==true)
			{
				
				//Get text length so we can calculate needed width
				DWORD		captionLength=strlen(caption);
				SIZE		textProperties;

				HDC canvas = GetDC(myHwnd);
				GetTextExtentPoint32(canvas,caption,captionLength,&textProperties);
				ReleaseDC(myHwnd,canvas);

				//Resize panel 
				setPanelWidth(index,textProperties.cx);
			}
		}
	}

	//Update text

	//The | 0 bit just sets the drawing border type to a sunken border
	sendMsg(SB_SETTEXT,index | 0,(LPARAM)caption);
}

char* CGadgetStatusbar::getPanelText(int index)
{
	//Get length of text so we only allocate required amount of memory
	int textLen=sendMsg(SB_GETTEXTLENGTH,index,0);

	char* text=new char[textLen+1];

	sendMsg(SB_GETTEXT,index,(LPARAM)text);

	return text;
}



void CGadgetStatusbar::setPanelWidth(DWORD index,DWORD width)
{
	//Get current part count
	DWORD partCount=getPanelCount();

	//Check that index is valid
	if (index >= partCount)
	{
		return;
	}

	//Get part details into array
	int* parts=new int[partCount];
	sendMsg(SB_GETPARTS,partCount,(LPARAM)parts);

	//Get difference in old and new widths so we can resize panels with a higher index
	int widthDifference=width-(*(parts+index));

	//Adjust width of specified part - add width parameter onto right-edge co-ord of previous item
	*(parts+index)=*(parts+index-1)+width;

	//Adjust right-coords of other panels to ensure that they keep their widths
	for (DWORD k=index+1;k < partCount;k++)
	{
		*(parts+k)=*(parts+k)+widthDifference;
	}

	//Update status bar with new array widths
	sendMsg(SB_SETPARTS,partCount,(LPARAM)parts);

	//Clean up
	delete [] parts;
}

void CGadgetStatusbar::removePanel(DWORD index)
{
	//Get current panel count
	DWORD partCount=getPanelCount();

	//Check that index is valid
	if (index >= partCount)
	{
		return;
	}

	//Get details of existing items into array
	int* existingParts=new int[partCount];
	sendMsg(SB_GETPARTS,partCount,(LPARAM)existingParts);

	char* partText=new char[127];

	DWORD i=0;

	//Shuffle the text up 
	for (i=index;i<(partCount);i++)
	{
		sendMsg(SB_GETTEXT,i+1,(LPARAM)partText);
		sendMsg(SB_SETTEXT,i | 0,(LPARAM)partText);
	}

	delete [] partText;

	//Create new array to hold widths except with specified panel deleted
	int* newParts=new int[partCount-1];

	i=0;

	DWORD offset=0;

	//Copy array items *except* the one with the index to be removed
	for (i=0;i<partCount;i++)
	{
		if (i != index)
		{
			//Use a seperate var to track the offset because if we used i, we would end up trying
			//to write beyond the end-point of the array
			*(newParts+offset)=*(existingParts+offset);
			offset++;
		}
	}

	//Update the widths in the control
	sendMsg(SB_SETPARTS,partCount-1,(LPARAM)newParts);

	//Clean up

	delete [] existingParts;
	delete [] newParts;
}

//CGadgetToolbar
//================================================================================================
bool CGadgetToolbar::create(HWND parent)
{
	//We have to create a rebar container control so that the bar displays correctly in DBPro.

	REBARINFO rebarProperties;
	REBARBANDINFO rebarBandProperties;
	
	//rebar=CreateWindow("STATIC",NULL,WS_CHILD|WS_VISIBLE,0,0,500,20,parent,0,CApplication::core->hInstance,0);
	

	rebar=CreateWindowEx(WS_EX_TOOLWINDOW,REBARCLASSNAME,NULL,defaultStyles|
		RBS_AUTOSIZE|CCS_NODIVIDER,0,0,0,0,parent,0,0,0);


		if (!rebar)
			{
				debugInt("Creation of Rebar Failed",(int)GetLastError());
				return NULL;
			}

			
			// Initialize and send the REBARINFO structure.
			ZeroMemory(&rebarProperties,sizeof(REBARINFO));
			rebarProperties.cbSize = sizeof(REBARINFO);  // Required when using this struct.
			rebarProperties.fMask  = 0;
			rebarProperties.himl   = (HIMAGELIST)NULL;
  
			//Exit if configuration of rebar fails
			if(!SendMessage(rebar, RB_SETBARINFO, 0, (LPARAM)&rebarProperties))
			{
				debugInt("Configuration of Rebar Failed",(int)GetLastError());
			 return NULL;
			}
			
			// Initialize structure members that both bands will share.
			rebarBandProperties.cbSize = sizeof(REBARBANDINFO);  // Required
			rebarBandProperties.fMask  = RBBIM_COLORS | RBBIM_TEXT | RBBIM_BACKGROUND | 
                   RBBIM_STYLE | RBBIM_CHILD  | RBBIM_CHILDSIZE | 
                   RBBIM_SIZE;
			rebarBandProperties.fStyle = RBBS_CHILDEDGE | RBBS_FIXEDBMP;
			rebarBandProperties.hbmBack= NULL;


			//Finally create our toolbar window
			myHwnd=CreateWindowEx(NULL,TOOLBARCLASSNAME,NULL,WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS|TBSTYLE_LIST
			,0,0,75,25,parent,0,NULL,0);
//WIN98FIX

			if (CApplication::isWindowsNT)
			{
				sendMsg(TB_SETEXTENDEDSTYLE,0,(LPARAM)(DWORD)TBSTYLE_EX_MIXEDBUTTONS); //This allows us to add tooltips
			}

			ShowWindow(myHwnd,SW_SHOWNORMAL);

			//Get size of toolbar buttons so we can ensure that rebar is always correct height
			DWORD toolbarButtonHeight=sendMsg(TB_GETBUTTONSIZE,0,0);

	
			// Set values unique to the band with the toolbar.
					rebarBandProperties.lpText     = "Tool Bar";
					rebarBandProperties.hwndChild  = myHwnd;
					rebarBandProperties.cxMinChild = 0;
					rebarBandProperties.cyMinChild = HIWORD(toolbarButtonHeight);
					//rebarBandProperties.cyMinChild = 50;
					rebarBandProperties.cx         = 250;

					//TODO - This really should use the official system colour rather than hard-coded light grey
					rebarBandProperties.clrBack	   = GetSysColor(COLOR_3DFACE);

			// Add the band that has the toolbar.
					SendMessage(rebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rebarBandProperties);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Tell the toolbar what version of Common Controls we are using by telling it the size of the 
		//button structure
		sendMsg(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0); 


		//debugInt("SM",(int)sendMsg(TB_ADDSTRING,0,(LPARAM)"Test String\0\0"));
		

		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);


		return true;
	}
	else
	{
		debugInt("CGadgetToolbar::create",(int)GetLastError());
		return false;
	}
}

void CGadgetToolbar::destroy()
{
	CGadget::destroy();

	DestroyWindow(rebar);
	//debugInt("rebar",(int)rebar);
}

void CGadgetToolbar::addButton(int index, bool makeCheckButton, char* tooltipText)
{
	//Create a new button structure
	TBBUTTON	newButton;
	int			currentButtonCount=(int)sendMsg(TB_BUTTONCOUNT,0,0);

	//In this version of addButton, we assume that the bitmap has already been added to the toolbar, so we just create a new button and 
	//specify index as the bitmap to use
	newButton.iBitmap=index;

	//By setting the command value of the new button (the value returned by getHotItem) to the current button count of the toolbar,
	//we get a zero-based index (1st button has index 0, 2nd has index 1 etc.) when using getHotItem, so if getHotItem returned 3 after
	//clicking on the toolbar, we would have the fourth item
	newButton.idCommand=currentButtonCount;
	newButton.fsState=TBSTATE_ENABLED;

	//If we are creating a push-down button - if for example we were doing Bold/Italic/Underline buttons in a text editor,
	//then makeCheckButton will be true, and we use a different style for the button
	if (makeCheckButton)
	{
//WIN98FIX
		newButton.fsStyle=TBSTYLE_CHECK;
	}
	else
	{
//WIN98FIX
		newButton.fsStyle=TBSTYLE_BUTTON;
	}
	newButton.fsStyle=newButton.fsStyle|TBSTYLE_AUTOSIZE;

	newButton.dwData=0;

	if (CApplication::isWindowsNT)
	{
		newButton.iString=(INT_PTR)tooltipText;
	}
	else
	{
		newButton.iString=0;
	}

	//Syntax: sendMsg(TB_ADDBUTTONS,Number of buttons to add,Point to array of new buttons)
	sendMsg(TB_ADDBUTTONS,1,(LPARAM)&newButton);
}

void CGadgetToolbar::setVisible(DWORD flag, BOOL centerInParent)
{
	//Toolbars are different - they must show or hide rebar housing
	if (flag)
	{
		ShowWindow(rebar,SW_SHOWNORMAL);
	}
	else
	{
		ShowWindow(rebar,SW_HIDE);
	}
}

void CGadgetToolbar::addSplitter()
{
	//Create a new button strucutre
	TBBUTTON	newButton;
	int			currentButtonCount=(int)sendMsg(TB_BUTTONCOUNT,0,0);
//WIN98FIX
	newButton.fsStyle=TBSTYLE_SEP;
	newButton.idCommand=currentButtonCount;
	newButton.fsState=TBSTATE_ENABLED;
	newButton.dwData=0;
	newButton.iString=0;
	newButton.iBitmap=0;

	sendMsg(TB_ADDBUTTONS,1,(LPARAM)&newButton);
}

void CGadgetToolbar::addButton(char* fileName,bool makeCheckButton, char* tooltipText)
{
	//Load the bitmap from the specified file
	HBITMAP bitmap;
	//Note that we can't delete this handle unless we replace the bitmap, because otherwise the toolbar will be 
	//destroyed
	bitmap=(HBITMAP)LoadImage(NULL,fileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_LOADMAP3DCOLORS|LR_LOADTRANSPARENT);

	BITMAP bitmapInfo;
	GetObject((HGDIOBJ)bitmap,sizeof(bitmapInfo),&bitmapInfo);

	
	//Set bitmap size to that of first bitmap added to the toolbar, subsequent calls are ignored.
	if (sendMsg(TB_BUTTONCOUNT,0,0) == 0)
	{
		//Change toolbar button size to that of bitmap
		sendMsg(TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(bitmapInfo.bmWidth,bitmapInfo.bmHeight));
		//Update the toolbar
		sendMsg(TB_AUTOSIZE,0,0);

		//Change the rebar band so that the minimum height is set to the button height
		REBARBANDINFO bandInfo;
		bandInfo.cbSize=sizeof(REBARBANDINFO);
		bandInfo.fMask=RBBIM_CHILDSIZE;
		sendMsg(RB_GETBANDINFO,0,(LPARAM)&bandInfo);
		bandInfo.cyMinChild=HIWORD(sendMsg(TB_GETBUTTONSIZE,0,0));
		sendMsg(RB_SETBANDINFO,0,(LPARAM)&bandInfo);
		sendMsg(RB_SHOWBAND,0,0);
		
	}
	

	//If the bitmap was loaded successfully then add a new bitmap to the toolbar
	//and then add a new button with that bitmap
	if (bitmap)
	{
		TBADDBITMAP newBitmap;
		newBitmap.hInst=NULL;

		newBitmap.nID=(UINT_PTR)bitmap;
		
		int bitmapId=sendMsg(TB_ADDBITMAP,1,(LPARAM)&newBitmap);
		
		//If the bitmap was added successfully, create a new button
		if (bitmapId != -1)
		{
			//Same as other addButton function
			TBBUTTON	newButton;
			int			currentButtonCount=(int)sendMsg(TB_BUTTONCOUNT,0,0);

			//Set the bitmap to the id of the newly added bitmap
			newButton.iBitmap=bitmapId;
			newButton.idCommand=currentButtonCount;

			newButton.fsState=TBSTATE_ENABLED;
			
			
			//If we are making a push-in "toggle" button, use a different style
			if (makeCheckButton)
			{
	//WIN98FIX
					newButton.fsStyle=TBSTYLE_CHECK;
			}
			else
			{
//WIN98FIX
				newButton.fsStyle=TBSTYLE_BUTTON;
			}

			newButton.fsStyle=newButton.fsStyle|TBSTYLE_AUTOSIZE;
			newButton.dwData=0;

			if (CApplication::isWindowsNT)
			{
				newButton.iString=(INT_PTR)tooltipText;
			}
			else
			{
				newButton.iString=0;
			}


			//Syntax: sendMsg(TB_ADDBUTTONS,Number of buttons to add,Pointer to array of new buttons)
			sendMsg(TB_ADDBUTTONS,1,(LPARAM)&newButton);
		}
	}

}

void CGadgetToolbar::addStandardButtons()
{
	//Various buttons such as File Open/Save, Cut/Copy/Paste etc. are built into the common controls library, 
	//calling this function adds the common controls bitmaps to the toolbar

	TBADDBITMAP newBitmap;

	newBitmap.hInst=HINST_COMMCTRL;
	newBitmap.nID=IDB_STD_SMALL_COLOR;

	sendMsg(TB_ADDBITMAP,1,(LPARAM)&newBitmap);
}

void CGadgetToolbar::removeButton(int index)
{
	sendMsg(TB_DELETEBUTTON,(WPARAM)index,0);
}

void CGadgetToolbar::setButtonEnabled(int index, int enabled)
{
	sendMsg(TB_ENABLEBUTTON,(WPARAM)index,(LPARAM)enabled);
}

int CGadgetToolbar::getButtonEnabled(int index)
{
	TBBUTTONINFO buttonInfo;
	buttonInfo.cbSize=sizeof(TBBUTTONINFO);
	buttonInfo.dwMask=TBIF_STATE;

	sendMsg(TB_GETBUTTONINFO,(WPARAM)index,(LPARAM)&buttonInfo);

	if (buttonInfo.fsState & TBSTATE_ENABLED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CGadgetToolbar::setButtonState(int index, int state)
{
	sendMsg(TB_CHECKBUTTON,(WPARAM)index,(LPARAM)state);
}

int CGadgetToolbar::getButtonState(int index)
{
	TBBUTTONINFO buttonInfo;
	buttonInfo.cbSize=sizeof(TBBUTTONINFO);
	buttonInfo.dwMask=TBIF_STATE;

	sendMsg(TB_GETBUTTONINFO,(WPARAM)index,(LPARAM)&buttonInfo);

	if (buttonInfo.fsState & TBSTATE_CHECKED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CGadgetToolbar::getHotItem()
{
	//Returns the item which the gadget is currently over
	return sendMsg(TB_GETHOTITEM,0,0);
}

//CGadgetUpDown
//================================================================================================

bool CGadgetUpDown::create(int x, int y, int width, int height, int style, HWND parent)
{
	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created

	DWORD orientation=0;

	if (style==1)
	{
		orientation=UDS_HORZ;
	}

	myHwnd=CreateWindow(UPDOWN_CLASS,0,defaultStyles|WS_BORDER|UDS_ALIGNRIGHT|UDS_NOTHOUSANDS|UDS_SETBUDDYINT|orientation,x,y,width,height,parent,0,NULL,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		sendMsg(UDM_SETRANGE32,0,100); //Set default range 0-100

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetUpDown::setBuddy(HWND buddy)
{
	sendMsg(UDM_SETBUDDY,(WPARAM)buddy,0);
}

HWND CGadgetUpDown::getBuddy()
{
	return (HWND)sendMsg(UDM_GETBUDDY,0,0);
}

void CGadgetUpDown::setRange(int min, int max)
{
	sendMsg(UDM_SETRANGE32,min,max);
}

void CGadgetUpDown::setPosition(int position)
{
//WIN98FIX
	sendMsg(UDM_SETPOS,0,(LPARAM)MAKELONG((short)position,0));
}

int CGadgetUpDown::getMaximumValue()
{
	int maxValue=0;

	sendMsg(UDM_GETRANGE32,0,(LPARAM)&maxValue);

	return maxValue;
}

int CGadgetUpDown::getMinimumValue()
{
	int minValue=0;

	sendMsg(UDM_GETRANGE32,(WPARAM)&minValue,0);

	return minValue;
}

int CGadgetUpDown::getPosition()
{
	BOOL errorOccurred=0;
	
//WIN98FIX
	int position=(int)LOWORD(sendMsg(UDM_GETPOS,0,0));

	if (!errorOccurred)
	{
		return position;
	}
	else
	{
		return 0;
	}
}

//CGadgetWindow
//================================================================================================
bool CGadgetWindow::create(int x, int y, int width, int height, char* caption, DWORD styleOptions, DWORD exStyleOptions, int createTopMost, HWND parent)
{
	//Is the window a child window or a topmost window?
	HWND parentWindow;

	if (createTopMost)
	{
		parentWindow=0;
	}
	else
	{
		parentWindow=parent;
	}

	//Init vars
	myIcon=0;

	//Creating with the WS_VISIBLE style means that the gadget will be shown as soon as it is created
	myHwnd=CreateWindowEx(exStyleOptions,"CGADGETWINDOW",caption,styleOptions|WS_CLIPCHILDREN|(defaultStyles&WS_VISIBLE),x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		return true;
	}
	else
	{
	//	debugInt("LastError",(int)GetLastError());
		return false;
	}
}

void CGadgetWindow::minimize()
{
	ShowWindow(myHwnd,SW_MINIMIZE);
}

void CGadgetWindow::maximize()
{
	ShowWindow(myHwnd,SW_MAXIMIZE);
}

void CGadgetWindow::restore()
{
	ShowWindow(myHwnd,SW_RESTORE);
}

void CGadgetWindow::setIcon(char* fileName)
{
	//Destroy existing icon if it already exists to prevent memory wastage
	if (myIcon)
	{
		DestroyIcon(myIcon);
	}

	myIcon=(HICON)LoadImage(NULL,fileName,IMAGE_ICON,16,16,LR_LOADFROMFILE);

	if (myIcon)
	{
		sendMsg(WM_SETICON,ICON_SMALL,(LPARAM)myIcon);
	}

	
}

//CGadgetTrackbar
//================================================================================================

bool CGadgetTrackbar::create(int x, int y, int width, int height, int style, HWND parent)
{
	if (style==0) // Horizontal
	{
		myHwnd=CreateWindowEx(NULL,TRACKBAR_CLASS,0,defaultStyles|TBS_AUTOTICKS|TBS_HORZ,x,y,width,height,parent,0,0,0);
	}

	if (style==1) // Vertical
	{
		myHwnd=CreateWindowEx(NULL,TRACKBAR_CLASS,0,defaultStyles|TBS_AUTOTICKS|TBS_VERT,x,y,width,height,parent,0,0,0);
	}

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		//Set default values
		setRange(0,10);
		setTickFrequency(1);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetTrackbar::setRange(int min, int max)
{
	sendMsg(TBM_SETRANGEMIN,FALSE,(LPARAM)min);
	sendMsg(TBM_SETRANGEMAX,TRUE,(LPARAM)max);
}

void CGadgetTrackbar::setTickFrequency(int frequency)
{
	sendMsg(TBM_SETTICFREQ,(WPARAM)frequency,0);
}

void CGadgetTrackbar::setPosition(int position)
{
	sendMsg(TBM_SETPOS,TRUE,(LPARAM)position);
}

int CGadgetTrackbar::getPosition()
{
	return sendMsg(TBM_GETPOS,0,0);
}

int CGadgetTrackbar::getMinimumValue()
{
	return sendMsg(TBM_GETRANGEMIN,0,0);
}

int CGadgetTrackbar::getMaximumValue()
{
	return sendMsg(TBM_GETRANGEMAX,0,0);
}

//CGadgetCanvas
//================================================================================================

bool CGadgetCanvas::create(int x, int y, int width, int height, HWND parent)
{
	myHwnd=CreateWindowEx(0,"STATIC",0,defaultStyles|WS_CLIPSIBLINGS|SS_NOTIFY|SS_OWNERDRAW,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		setType(GADGET_CANVAS);

		//Init vars
		myMemoryDCWidth=0;
		myMemoryDCHeight=0;
		gadgetDC=0;
		myFont=0;
		myBrush=0;
		myPen=0;
		myAlpha=255; //255 = Opaque, 0 = Transparent

		//Create the internal memory DC with the specified width & height
		initDC(width,height);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetCanvas::initDC(int width, int height)
{
	//TODO - Resizing the DC currently resets the colours

	int oldPenWidth=getPenWidth();
	

	//Return if nothing has changed
	if ((width == myMemoryDCWidth) || (height == myMemoryDCHeight))
	{
		return;
	}

	//Delete old DC, old Pen, old Brush & old Bitmap if defined

	//Select font out of DC
	SelectObject(gadgetDC,GetStockObject(DEFAULT_GUI_FONT));

	if (myPen)
	{
		SelectObject(gadgetDC,GetStockObject(BLACK_PEN));
		DeleteObject(myPen);
	}

	if (myBrush)
	{
		SelectObject(gadgetDC,GetStockObject(NULL_BRUSH));
		DeleteObject(myBrush);
	}

	if (gadgetImage)
	{
		DeleteObject(gadgetImage);
	}

	if (gadgetDC)
	{
		DeleteObject(gadgetDC);
	}




	//Obtain a handle to the window's device context
	HDC winDC=GetDC(myHwnd);

	//Create a compatible DC to draw into
	gadgetDC=CreateCompatibleDC(winDC);
	RealizePalette(gadgetDC);

	//Create a compatible bitmap
	gadgetImage=CreateCompatibleBitmap(winDC,width,height);



	//Place bitmap into memory DC
	SelectObject(gadgetDC,gadgetImage);

	//Select pen, brush & font into memory DC
	if (!myFont)
	{
		myFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}

	myPen=CreatePen(PS_SOLID,oldPenWidth,0);
	myBrush=CreateSolidBrush(0);

	SelectObject(gadgetDC,myPen);
	SelectObject(gadgetDC,myBrush);
	SelectObject(gadgetDC,myFont);

	//Erase the canvas (not needed under XP, but results in a nasty memory mess
	//in Win98 otherwise)
	drawRect(0,0,width,height);

	//Go back to a white brush & pen
	setDrawingColor(RGB(255,255,255),RGB(255,255,255),0);

	//Update width & height
	myMemoryDCWidth=width;
	myMemoryDCHeight=height;

	//Release the window's device context
	ReleaseDC(myHwnd,winDC);
}

void CGadgetCanvas::setDrawingColor(COLORREF lineColor, COLORREF fillColor, COLORREF textColor)
{
	//Copy handles to old pen & brush
	HPEN oldPen=myPen;
	HBRUSH oldBrush=myBrush;

	//Create new pen & brush
	myPen=CreatePen(PS_SOLID,getPenWidth(),lineColor);
	myBrush=CreateSolidBrush(fillColor);

	//Select objects into memory DC
	SelectObject(gadgetDC,myPen);
	SelectObject(gadgetDC,myBrush);

	SetTextColor(gadgetDC,textColor);
	SetBkColor(gadgetDC,fillColor);

	//Free old resources
	DeleteObject(oldPen);
	DeleteObject(oldBrush);

}

int CGadgetCanvas::getPenWidth()
{
	//If pen has not yet been created, return 1 so a single pixel wide pen is created
	if (!myPen) 
	{
		return 1;
	}

	LOGPEN penInfo;

	GetObject((HGDIOBJ)myPen,sizeof(LOGPEN),&penInfo);

	return penInfo.lopnWidth.x;
}

void CGadgetCanvas::setLineWidth(int width)
{
	//Get information about current pen
	LOGPEN penInfo;

	GetObject((HGDIOBJ)myPen,sizeof(LOGPEN),&penInfo);

	DeleteObject((HGDIOBJ)myPen);

	myPen=CreatePen(PS_SOLID,width,penInfo.lopnColor);

	SelectObject(gadgetDC,(HGDIOBJ)myPen);
}

void CGadgetCanvas::copyFrom(CGadgetCanvas* source, int left, int top, int right, int bottom)
{
	BitBlt(gadgetDC,0,0,(right-left),(bottom-top),source->getCanvasDC(),left,top,SRCCOPY);
}

HDC CGadgetCanvas::getCanvasDC()
{
	return gadgetDC;
}

void CGadgetCanvas::setColors(COLORREF background, COLORREF text)
{
	setDrawingColor(background,background,text);

	//Draw a rect to fill the whole canvas
	drawRect(0,0,getWidth(),getHeight());

	//Update the canvas on screen
	update();
}

void CGadgetCanvas::drawRect(int x1, int y1, int x2, int y2)
{
	Rectangle(gadgetDC,x1,y1,x2,y2);
}

void CGadgetCanvas::drawLine(int x1, int y1, int x2, int y2)
{
	MoveToEx(gadgetDC,x1,y1,0);
	LineTo(gadgetDC,x2,y2);
}

void CGadgetCanvas::drawEllipse(int x1, int y1, int x2, int y2)
{
	Ellipse(gadgetDC,x1,y1,x2,y2);
}



void CGadgetCanvas::drawText(int x, int y, char* text)
{
	//Make Text Transparent
	SetBkMode(gadgetDC,TRANSPARENT);
	ExtTextOut(gadgetDC,x,y,0,0,text,strlen(text),0);	

}

void CGadgetCanvas::drawPoint(int x, int y, COLORREF color)
{
	SetPixelV(gadgetDC,x,y,color);
}

void CGadgetCanvas::drawCanvas()
{	
	//Copy memory DC contents to window DC
	HDC winDC=GetDC(myHwnd);

	BitBlt(winDC,0,0,getWidth(),getHeight(),gadgetDC,0,0,SRCCOPY);

	//--Not Working Yet--
	//Use alpha blending to make canvas appear translucent if the alpha value has been set.
	//updateAlpha(winDC);

	ReleaseDC(myHwnd,winDC);
}

void CGadgetCanvas::updateAlpha(HDC winDC)
{
	//If alpha value is 255 (opaque) then no blending is necessary, so exit.
	if (myAlpha >= 255)
	{
		return;
	}

	//Get co-ords of canvas relative to parent
	int x=getX();
	int y=getY();

	//Get size of canvas
	int width=getWidth();
	int height=getHeight();

	//Get a handle to the parent window's DC
	HWND parentWin=GetParent(myHwnd);

	
	HDC  parentDC=GetDC(parentWin);

	//Prepare BLENDINFO structure with current canvas alpha info
	BLENDFUNCTION blendInfo;
	
	blendInfo.BlendOp=AC_SRC_OVER;
	blendInfo.BlendFlags=0;
	blendInfo.AlphaFormat=AC_SRC_ALPHA;
	blendInfo.SourceConstantAlpha=myAlpha;

	//Alpha blend the canvas DC and the parent HDC
	//CApplication::alphaBlend(winDC,0,0,width,height,parentDC,x,y,width,height,blendInfo);

	//Release the parent window's DC
	ReleaseDC(parentWin,parentDC);
}

COLORREF CGadgetCanvas::getPoint(int x, int y)
{
	return GetPixel(gadgetDC,x,y);
}

void CGadgetCanvas::setCanvasFont(CGadgetFont* font)
{
	LOGFONT fontInfo;

	GetObject((HGDIOBJ)font->getHandle(),sizeof(LOGFONT),&fontInfo);

	HFONT newFont=CreateFontIndirect(&fontInfo);

	SelectObject(gadgetDC,(HGDIOBJ)newFont);

	DeleteObject((HGDIOBJ)myFont);

	myFont=newFont;

}

void CGadgetCanvas::pasteImage(int destX, int destY, int imageId)
{
	core = CApplication::core;

	
	PFMakeMemblockFromImage makeMemblockFromImage	= CApplication::makeMemblockFromImage;
	PFMakeImageFromMemblock makeImageFromMemblock	= CApplication::makeImageFromMemblock;
	PFMakeMemblock			makeMemblock			= CApplication::makeMemblock;
	PFMemblockExists		memblockExists			= CApplication::memblockExists;
	PFGetMemblockPtr		getMemblockPtr			= CApplication::getMemblockPtr;
	PFGetMemblockSize		getMemblockSize			= CApplication::getMemblockSize;
	PFDeleteMemblock		deleteMemblock			= CApplication::deleteMemblock;
	PFImageExists			imageExists				= CApplication::imageExists;

	//Exit function if any of the functions were not found in the memblocks / image DLLs	
	if 
	(
		(!makeMemblockFromImage)	|| 
		(!makeImageFromMemblock)	||
		(!memblockExists)			|| 
		(!getMemblockPtr)			||
		(!getMemblockSize)			||
		(!deleteMemblock)			||
		(!imageExists)				||
		(!makeMemblock)
	)
	{
		return;
	}
	
	if (!imageExists(imageId))
		return;


	int memblockId;
	for (memblockId=1;memblockId < 256;memblockId++)
		if (!memblockExists(memblockId))
			break;

	if (memblockId == 256)
		return ; //Couldn't find a valid slot

	makeMemblockFromImage(memblockId,imageId);

	DWORD* memblockPtr=(DWORD*)getMemblockPtr(memblockId);

	int imageWidth = memblockPtr[0];
	int imageHeight = memblockPtr[1];
	int imageDepth = memblockPtr[2];
	
	int index=2;
	COLORREF color;

	for (int x=0;x<imageWidth;x++)
		for (int y=0;y<imageHeight;y++)
		{
			if (imageDepth == 32)
			{
				color = memblockPtr[++index];
				drawPoint(destX+x,destY+y,color);
			}
		}

	deleteMemblock(memblockId);
}

void CGadgetCanvas::getImage(int imageId)
{
	//Get DBPro core pointer so we can get memblocks & images function pointers.
#ifdef DBPRO_DLL
	core = CApplication::core;
#endif

	PFMakeMemblockFromImage makeMemblockFromImage	= CApplication::makeMemblockFromImage;
	PFMakeImageFromMemblock makeImageFromMemblock	= CApplication::makeImageFromMemblock;
	PFMakeMemblock			makeMemblock			= CApplication::makeMemblock;
	PFMemblockExists		memblockExists			= CApplication::memblockExists;
	PFGetMemblockPtr		getMemblockPtr			= CApplication::getMemblockPtr;
	PFGetMemblockSize		getMemblockSize			= CApplication::getMemblockSize;
	PFDeleteMemblock		deleteMemblock			= CApplication::deleteMemblock;
	PFImageExists			imageExists				= CApplication::imageExists;

	//Exit function if any of the functions were not found in the memblocks / image DLLs	
	if 
	(
		(!makeMemblockFromImage)	|| 
		(!makeImageFromMemblock)	||
		(!memblockExists)			|| 
		(!getMemblockPtr)			||
		(!getMemblockSize)			||
		(!deleteMemblock)			||
		(!imageExists)				||
		(!makeMemblock)
	)
	{
		return;
	}
	
	//Function pointers valid - do clever stuff :)

	//First check that the image does not exist
	if (imageExists(imageId))
	{
		return;
	}

	//Then find a free memblock ID
	DWORD memblockId=1;

	while (memblockExists(memblockId))
	{
		memblockId++;
	}
	//DBPro only allows up to 255 memblocks
	if (memblockId > 255) 
	{
		return;
	}

	//Create a new memblock
	//Size = 12 bytes for header + 4 bytes per pixel (RGBA)
	makeMemblock(memblockId,12+myMemoryDCWidth*myMemoryDCHeight*4);

	//Get a pointer to the memblock
	DWORD* memblockPtr=(DWORD*)getMemblockPtr(memblockId);

	//Write header (Width [4], Height [4], Depth [4])
	*memblockPtr=myMemoryDCWidth;
	*(memblockPtr+1)=myMemoryDCHeight;
	*(memblockPtr+2)=32; //32 bit depth image = 4 bytes per pixel

	DWORD offset=3; //Position offset to first DWORD past 3 DWORD (12 byte) header

	DWORD* imageBuffer=getImageDataPtr();

	//Write image data
	for (DWORD y=0;y<(DWORD)myMemoryDCHeight;y++)
	{
		for (DWORD x=0;x<(DWORD)myMemoryDCWidth;x++)
		{
			//Get colour, convert it to DBPro format and store it in the memblock
			*(memblockPtr+offset)=(*(imageBuffer+offset-3));
			//Increment offset to write the next pixel
			offset++;
		}
	}

	//Release image buffer
	free(imageBuffer);

	//Make an image from the memblock
	makeImageFromMemblock(imageId,memblockId);

	//Tidy up - delete the memblock
	deleteMemblock(memblockId);
}

//CodeGuru source
//http://216.239.59.104/search?q=cache:Ty8igVaoc_4J:www.codeguru.com/Cpp/G-M/gdi/article.php/c3693/+GetPixel+speed&hl=en
DWORD* CGadgetCanvas::getImageDataPtr()
{
	BITMAPINFO bi;
	BOOL bRes;
	DWORD* buf;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = myMemoryDCWidth;
	bi.bmiHeader.biHeight = - myMemoryDCHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = myMemoryDCWidth * 4 * myMemoryDCHeight;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	buf = (DWORD*) malloc(myMemoryDCWidth * 4 * myMemoryDCHeight);
	bRes = GetDIBits(gadgetDC, gadgetImage, 0, myMemoryDCHeight, buf, &bi,
                   DIB_RGB_COLORS);
	if (!bRes) 
	{
		free(buf);
		buf = 0;
	}
	return buf;
}

//CGadgetListview
//================================================================================================

bool CGadgetListview::create(int x, int y, int width, int height, HWND parent)
{
	myHwnd=CreateWindowEx(NULL,WC_LISTVIEW,0,defaultStyles|WS_CLIPSIBLINGS|LVS_SHAREIMAGELISTS|LVS_ICON|LVS_SINGLESEL,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);
		
		myColumnCount=0;

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetListview::onSize(int width,int height)
{
	ListView_Arrange(myHwnd,LVA_DEFAULT);
}

//listIndex, 0=Normal, 1=Small, 2=State
void CGadgetListview::setImageList(CGadgetImageList *imageList, int listIndex)
{
	
	ListView_SetImageList(myHwnd,imageList->getHImageList(),listIndex);
}

void CGadgetListview::addItem(char* caption, int image)
{
	LVITEM newItem;

	newItem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	newItem.iItem=ListView_GetItemCount(myHwnd); //Set index to first unused index
	newItem.iSubItem=0;
	newItem.state=0;
	newItem.stateMask=0;
	newItem.iImage=image;
	newItem.pszText=caption;

	ListView_InsertItem(myHwnd,&newItem);

}

void CGadgetListview::setItemText(char* caption, int index, int subItemIndex)
{
	LVITEM newItem;

	newItem.mask=LVIF_TEXT;
	newItem.iItem=index;
	newItem.iSubItem=subItemIndex;
	newItem.pszText=caption;

	ListView_SetItem(myHwnd,&newItem);
}

void CGadgetListview::setViewStyle(int viewStyle)
{
	if (viewStyle <= LVS_TYPEMASK)
	{
		// Retrieve the current window style. 
		DWORD existingStyle = GetWindowLong(myHwnd, GWL_STYLE); 
 
		// Only set the window style if the view bits have changed. 
		if ((existingStyle & LVS_TYPEMASK) != viewStyle) 
		{
			SetWindowLong(myHwnd, GWL_STYLE,(existingStyle & ~LVS_TYPEMASK) | viewStyle); 
		}
	}
	else
	{
		ListView_SetView(myHwnd,viewStyle);
	}
}

void CGadgetListview::setColors(COLORREF background, COLORREF text)
{
	ListView_SetBkColor(myHwnd,background);
	ListView_SetTextBkColor(myHwnd,background);
	ListView_SetTextColor(myHwnd,text);
}

void CGadgetListview::addColumn(char* caption, int width)
{
	LVCOLUMN newColumn;

	newColumn.mask=LVCF_TEXT|LVCF_WIDTH;
	newColumn.cx=width;
	newColumn.iSubItem=-1;
	newColumn.iOrder=myColumnCount;
	newColumn.pszText=caption;
	
	ListView_InsertColumn(myHwnd,myColumnCount,&newColumn);

	myColumnCount++;
}

int CGadgetListview::getItemCount()
{
	return ListView_GetItemCount(myHwnd);
}

void CGadgetListview::removeItem(int index)
{
	if (index > -1)
	{
		ListView_DeleteItem(myHwnd,index);
	}
	else
	{
		ListView_DeleteAllItems(myHwnd);
	}

	ListView_Arrange(myHwnd,LVA_DEFAULT);
}

void CGadgetListview::selectItem(int index)
{
	ListView_SetSelectionMark(myHwnd,index);
}

int CGadgetListview::selectedItem()
{
	return ListView_GetSelectionMark(myHwnd);
}

int CGadgetListview::hitTestItem(int x, int y)
{
	LVHITTESTINFO hitInfo;

	hitInfo.pt.x=x;
	hitInfo.pt.y=y;

	ListView_SubItemHitTest(myHwnd,&hitInfo);

	return hitInfo.iItem;
}

int CGadgetListview::hitTestSubItem(int x, int y)
{
	LVHITTESTINFO hitInfo;

	hitInfo.pt.x=x;
	hitInfo.pt.y=y;

	ListView_SubItemHitTest(myHwnd,&hitInfo);

	return hitInfo.iSubItem;
}

int CGadgetListview::getItemX(int item, int subItem)
{
	RECT itemRect;

	ListView_GetSubItemRect(myHwnd,item,subItem,LVIR_BOUNDS,&itemRect);

	return itemRect.left;
}

int CGadgetListview::getItemY(int item, int subItem)
{
	RECT itemRect;

	ListView_GetSubItemRect(myHwnd,item,subItem,LVIR_BOUNDS,&itemRect);

	return itemRect.top;
}

void CGadgetListview::setColumnWidth(int column, int width)
{
	ListView_SetColumnWidth(myHwnd,column,width);
}

int CGadgetListview::getColumnWidth(int column)
{
	return ListView_GetColumnWidth(myHwnd,column);
}

void CGadgetListview::getItemText(int index, int subitem, char* buffer, int len)
{
	ListView_GetItemText(myHwnd,index,subitem,buffer,len);
}

//CGadgetTreeview
//================================================================================================
bool CGadgetTreeview::create(int x, int y, int width, int height, HWND parent)
{
	myHwnd=CreateWindowEx(NULL,WC_TREEVIEW,0,defaultStyles|TVS_HASLINES|TVS_LINESATROOT
							|TVS_HASBUTTONS,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		setType(GADGET_TREEVIEW);

		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);


		return true;
	}
	else
	{
		return false;
	}
}

HTREEITEM CGadgetTreeview::hitTest(int x, int y)
{
	TVHITTESTINFO testProperties;
	POINT coords;

	coords.x=x;
	coords.y=y;

	testProperties.pt=coords;
	testProperties.hItem=0; //Remember to zero this to save confusion if no item is under the mouse
	testProperties.flags=0;

	TreeView_HitTest(myHwnd,&testProperties);

	return testProperties.hItem;
}

void CGadgetTreeview::setImageList(CGadgetImageList* imageList, int index)
{
	TreeView_SetImageList(myHwnd,imageList->getHImageList(),index);
}

HTREEITEM CGadgetTreeview::addItem(HTREEITEM parentItem, int image, char* caption, HTREEITEM insertAfter)
{
	TV_INSERTSTRUCT newItem;
	TVITEM		itemInfo;
	TVITEMEX	exItemInfo;

	exItemInfo.mask=0;

	//TVITEM structure is obsolte, so we are just using it as a null placeholder
	itemInfo.mask=0;

	//Fill in details about new item
	exItemInfo.mask=TVIF_TEXT;
	exItemInfo.pszText=caption;

	exItemInfo.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	exItemInfo.iImage=image;
	exItemInfo.iSelectedImage=image;

	newItem.hParent=parentItem;

	if (insertAfter)
	{
		//Special value for insertAfter makes new item first in list
		if ((int)insertAfter==-1)
		{
			newItem.hInsertAfter=TVI_FIRST;
		}
		else
		{
			newItem.hInsertAfter=insertAfter;
		}
	}
	else
	{
		newItem.hInsertAfter=TVI_LAST;	
	}

	newItem.item=itemInfo;
	newItem.itemex=exItemInfo;

	return TreeView_InsertItem(myHwnd,&newItem);
}

void CGadgetTreeview::setColors(COLORREF background, COLORREF text)
{
	TreeView_SetBkColor(myHwnd,background);
	TreeView_SetLineColor(myHwnd,text);
	TreeView_SetTextColor(myHwnd,text);
}

void CGadgetTreeview::setItemText(HTREEITEM item, char* text)
{
	TVITEMEX itemInfo;

	itemInfo.mask=TVIF_HANDLE|TVIF_TEXT;
	itemInfo.hItem=item;
	itemInfo.pszText=text;

	TreeView_SetItem(myHwnd,&itemInfo);
}

void CGadgetTreeview::getItemText(HTREEITEM item, char* textBuffer, int bufferSize)
{
	TVITEMEX itemInfo;

	itemInfo.mask=TVIF_HANDLE|TVIF_TEXT;
	itemInfo.hItem=item;
	itemInfo.pszText=textBuffer;
	itemInfo.cchTextMax=bufferSize;

	TreeView_GetItem(myHwnd,&itemInfo);
}

void CGadgetTreeview::removeItem(HTREEITEM item)
{
	if (item==(HTREEITEM)-1)
	{
		TreeView_DeleteAllItems(myHwnd);
	}
	else
	{
		TreeView_DeleteItem(myHwnd,item);
	}
}

void CGadgetTreeview::selectItem(HTREEITEM item)
{
	TreeView_SelectItem(myHwnd,item);
	
}

HTREEITEM CGadgetTreeview::selectedItem()
{
	return TreeView_GetSelection(myHwnd);
}

HTREEITEM CGadgetTreeview::getRoot()
{
	return TreeView_GetRoot(myHwnd);
}

HTREEITEM CGadgetTreeview::getFirstChild(HTREEITEM item)
{
	return TreeView_GetChild(myHwnd,item);
}

HTREEITEM CGadgetTreeview::getNextSibling(HTREEITEM item)
{
	return TreeView_GetNextSibling(myHwnd,item);
}

HTREEITEM CGadgetTreeview::getPreviousSibling(HTREEITEM item)
{
	return TreeView_GetPrevSibling(myHwnd,item);
}

HTREEITEM CGadgetTreeview::getParent(HTREEITEM item)
{
	return TreeView_GetParent(myHwnd,item);
}

//CGadgetWebBrowser
//================================================================================================
bool CGadgetWebBrowser::create(int x, int y, int width, int height, HWND parent)
{
	myHwnd=CreateWindowEx(NULL,"CGADGETWEBBROWSER","Test Browser Gadget",defaultStyles,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		
		//(*lpDisplayHTMLPage)(myHwnd, "http://www.microsoft.com");

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetWebBrowser::navigate(char* URL)
{
	(*lpDisplayHTMLPage)(myHwnd, URL);
}

void CGadgetWebBrowser::currentURL(char* buffer, DWORD size)
{
	(*lpGetCurrentURL)(myHwnd,buffer,size);
}
   
void CGadgetWebBrowser::run(char* html)
{
	(*lpDisplayHTMLStr)(myHwnd,html);
}

void CGadgetWebBrowser::move(int xPos, int yPos, int width, int height, bool clientSize)
{
	//Call base class implementation
	CGadget::move(xPos,yPos,width,height,clientSize);


	//Special web browser resize actions
	//(*lpSetSize)(myHwnd,width,height);
}

//CGadgetProgressBar
//================================================================================================
bool CGadgetProgressBar::create(int x, int y, int width, int height, HWND parent)
{
	myHwnd=CreateWindowEx(0,PROGRESS_CLASS,0,defaultStyles,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetProgressBar::setRange(int minValue, int maxValue)
{
	sendMsg(PBM_SETRANGE,0,MAKELPARAM(minValue,maxValue));
}

void CGadgetProgressBar::setPosition(int position)
{
	sendMsg(PBM_SETPOS,(WPARAM)position,0);
}

void CGadgetProgressBar::setColors(COLORREF background, COLORREF text)
{
	sendMsg(PBM_SETBARCOLOR,0,(LPARAM)text);
	sendMsg(PBM_SETBKCOLOR,0,(LPARAM)background);
}

int CGadgetProgressBar::getMin()
{
	return (int)sendMsg(PBM_GETRANGE,TRUE,0);
}

int CGadgetProgressBar::getMax()
{
	return (int)sendMsg(PBM_GETRANGE,FALSE,0);
}

int CGadgetProgressBar::getPosition()
{
	return (int)sendMsg(PBM_GETPOS,0,0);
}


//CGadgetRichEdit
//================================================================================================
bool CGadgetRichEdit::create(int x, int y, int width, int height, HWND parent)
{
	

	myHwnd=CreateWindowEx(WS_EX_CLIENTEDGE,RICHEDIT_CLASS,0,defaultStyles|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_AUTOHSCROLL|WS_HSCROLL,x,y,width,height,parent,0,0,0);

	//Was the window created successfully?
	if (myHwnd)
	{
		//Subclass window so we can interact with it
		myOldWinProc=(WNDPROC)SetWindowLong(myHwnd,GWL_WNDPROC,(DWORD)(SubClassFunc));
		SetWindowLong(myHwnd,GWL_USERDATA,(LONG)this);

		sendMsg(EM_EXLIMITTEXT,0,0x7FFFFFFE); //Set max text length

		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetRichEdit::setWordWrapMode(int wordWrap)
{
	if (wordWrap)
	{
		
		sendMsg(EM_SETTARGETDEVICE,0,0);
	}
	else
	{
	//	SetWindowLong(myHwnd,GWL_STYLE,GetWindowLong(myHwnd,GWL_STYLE)|ES_AUTOHSCROLL|WS_HSCROLL);
		      // Let the window know of the changes
			sendMsg(EM_SETTARGETDEVICE,0,1);
     // SetWindowPos(myHwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
		//sendMsg(EM_SETTARGETDEVICE,0,1);
	}
}

void CGadgetRichEdit::setSelFont(CGadgetFont* font, COLORREF color)
{
	CHARFORMAT style;
	LOGFONT	   fontProperties;
	HDC		   screenDC;
	int		   fontTwipsHeight;

	//Retrieve information about font
	GetObject((HGDIOBJ)font->getHandle(),sizeof(LOGFONT),&fontProperties);

	//Convert height of font in pixels (LOGFONT) to height of font in twips (1/1400 of an inch)
		screenDC=GetDC(GetDesktopWindow());
	
		//Calculate the no. of twips high the font would be if it were 1 pixel / inch, then divide by the 
		//number of pixels that there actually are in an inch (96 in the case of the laptop I developed this on)
		fontTwipsHeight=(abs(fontProperties.lfHeight)*1400)/GetDeviceCaps(screenDC,LOGPIXELSY);

		ReleaseDC(GetDesktopWindow(),screenDC);

	//Set style options for formatting
	style.cbSize=sizeof(CHARFORMAT);
	style.dwMask=CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_COLOR|CFM_SIZE|CFM_FACE;

	style.yHeight=fontTwipsHeight;
	//style.szFaceName=fontProperties.lfFaceName;
	
	strcpy((char*)&style.szFaceName,fontProperties.lfFaceName);
	
	style.dwEffects=0;

	if (fontProperties.lfWeight > FW_NORMAL)
	{
		style.dwEffects |= CFE_BOLD;
	}

	if (fontProperties.lfItalic)
	{
		style.dwEffects |= CFE_ITALIC;
	}

	if (fontProperties.lfUnderline)
	{
		style.dwEffects |= CFE_UNDERLINE;
	}

	style.crTextColor = color;

	//Finally apply the character formatting to the current RichEdit selection

	sendMsg(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&style);
}


CGadgetFont* CGadgetRichEdit::getSelFont()
{
	//Get properties of currently selected text into CHARFORMAT structure
	CHARFORMAT style;
	
	style.cbSize=sizeof(CHARFORMAT);
	style.dwMask=CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_COLOR|CFM_SIZE|CFM_FACE;

	sendMsg(EM_GETCHARFORMAT,SCF_SELECTION,(LPARAM)&style);
	
	//Check which style flags are enabled in the currently selected font
	int bold;
	int italic;
	int underline;

	bold      = (style.dwEffects & CFE_BOLD);
	italic    = (style.dwEffects & CFE_ITALIC);
	underline = (style.dwEffects & CFE_UNDERLINE);

	//Calculate size (convert from Twips to Point Size)
	int  size;
    HDC  screenDC;

	screenDC=GetDC(GetDesktopWindow());
	int  pixelsPerInch=GetDeviceCaps(screenDC,LOGPIXELSY);
	ReleaseDC(GetDesktopWindow(),screenDC);

	//Add one to compensate for flooring effect of integer division
	size=((style.yHeight*72)/1400)+1;

	//Create a new CGadgetFont object with the properties of the currently selected text 
	CGadgetFont* newFont=new CGadgetFont;

	newFont->create((char*)&style.szFaceName,size,bold,italic,underline);

	return newFont;
}

COLORREF CGadgetRichEdit::getSelColor()
{
	//Get properties of currently selected text
	CHARFORMAT style;

	style.cbSize=sizeof(CHARFORMAT);
	style.dwMask=CFM_COLOR;

	sendMsg(EM_GETCHARFORMAT,SCF_SELECTION,(LPARAM)&style);

	//Return colour component of formatting

	return style.crTextColor;
}


void CGadgetRichEdit::setSel(int start, int len)
{
	sendMsg(EM_SETSEL,start,start+len);
}

void CGadgetRichEdit::setColors(COLORREF background, COLORREF text)
{
	//Note:  Text colour must be set by changing the font.

	sendMsg(EM_SETBKGNDCOLOR,0,background);
}

int CGadgetRichEdit::getSelStart()
{
	int start;
	int end;

	sendMsg(EM_GETSEL,(WPARAM)&start,(LPARAM)&end);

	return start;
}

int CGadgetRichEdit::getSelLen()
{
	int start;
	int end;

	sendMsg(EM_GETSEL,(WPARAM)&start,(LPARAM)&end);

	return end-start;
}

bool CGadgetButton::commandHandler(unsigned short commandCode)
{
	switch (commandCode)
	{
	case BN_CLICKED:
		CGadget::events->push( (DWORD)this, BGUI_BUTTONCLICK, 0, 0 );
		return true;
		break;
	}

	return false;
}

bool CGadgetCombobox::commandHandler(unsigned short commandCode)
{
	switch (commandCode)
	{
	case CBN_SELCHANGE:
		CGadget::events->push( (DWORD)this,BGUI_COMBOBOXCHANGE,0,0 );
		return true;
		break;
	}

	return false;
}

bool CGadgetTreeview::commandHandler(unsigned short commandCode)
{
	switch (commandCode)
	{
	case TVN_SELCHANGED:
		CGadget::events->push( (DWORD)this,BGUI_TREEVIEWCHANGE,0,0 );
		return true;
		break;
	}

	return false;
}

//SubClassFunction
//================================================================================================

LRESULT CALLBACK SubClassFunc(   HWND hWnd, UINT Message, WPARAM wParam, LONG lParam)
{
#ifdef DEMO_VERSION
	DWORD ticks=GetTickCount();

	if ( (ticks - CApplication::demoTimer > DEMO_TIMEOUT) && (!CApplication::timeout) )
	{
		CApplication::timeout=true;
		MessageBox(0,"The BlueGUI Trial Version has timed out.  The fully functional Blue Windows plugin with no time limitations can be purchased from http://blue.robert-knight.net.","Timeout",0);
		ExitProcess(0);
	}
#endif

	//Handling For All Windows, Gadget or Not
	//=======================================
	
	//If a size message is received, broadcast it to all children to make them resize as well
	//TODO - Possible performance implications?
	if (Message==WM_SIZE)
	{
			broadcastMessage(hWnd,WM_SIZE,0,0);
	}

	if (Message==WM_NOTIFY)
	{
		LPNMHDR event=(LPNMHDR)lParam;
		
		CGadget* child=(CGadget*)(GetWindowLong(event->hwndFrom,GWL_USERDATA));

		if (child)
		{
			child->handleNotification(event);
		}
	}

	if (Message==WM_COMMAND)
	{
		NMHDR event;
		event.code=HIWORD(wParam);
		event.hwndFrom=(HWND)lParam;
	
		CGadget* child=(CGadget*)(GetWindowLong(event.hwndFrom,GWL_USERDATA));

		if (child)
		{
			child->handleNotification(&event);
		}
	}

	//Retrieve pointer to gadget to which the window belongs
	CGadget* gadget = (CGadget*)(GetWindowLong(hWnd,GWL_USERDATA));

	//Process main window menu item - check that HIWORD(wParam) and lParam are zero 
	//because if they are not, the command may be coming from another gadget
	if ((Message==WM_COMMAND) && (lParam == 0) && (gadget==0))
	{
		//Note that eventData is modified for the convenience of the user to contain the menu item ID clicked
		
		//CGadget::events->push((DWORD)gadget,(DWORD)Message,(DWORD)wParam,0);
		CGadget::events->push( (DWORD)gadget,(DWORD)BGUI_MENUCLICK,(DWORD)wParam,0 );
	}

	//Intercept WM_COMMAND messages sent for child gadgets
	if ( (Message==WM_COMMAND) && (lParam != 0) )
	{
		CGadget* cmdGadget=(CGadget*)(GetWindowLong((HWND)lParam,GWL_USERDATA));

		if (cmdGadget)
		{
			if (!cmdGadget->commandHandler(HIWORD(wParam)))
			{
				CGadget::events->push((DWORD)cmdGadget,(DWORD)Message,(DWORD)HIWORD(wParam),(DWORD)lParam);
			}
		}
	}

	//Send a message out as the user selects different items
/*	if (Message==WM_MENUSELECT)
	{
		//Create a MENUINFO struct so that we can convert the menu handle into a CGadgetMenu object by
		//reading the dwMenuData property (set in CGadgetMenu::create)

		MENUINFO info;
		info.cbSize=sizeof(MENUINFO);
		info.fMask=MIM_MENUDATA;
		GetMenuInfo((HMENU)lParam,&info);

		//Prevent crashes if no menu item is specified
		if (lParam==0)
		{
			info.dwMenuData=0;
		}

		CGadget::events->push((DWORD)gadget,(DWORD)Message,(DWORD)LOWORD(wParam),info.dwMenuData);
	} */



	//Process custom colour messages
	if ((Message>=WM_CTLCOLOREDIT) && (Message<=WM_CTLCOLORSTATIC))
	{	
		//Retrive pointer to child gadget
		CGadget* child=(CGadget*)(GetWindowLong((HWND)lParam,GWL_USERDATA));

		if (child)
		{
			DWORD result=child->customPaint((HDC)wParam);

			if (result) { return result; }
		}
	}		

	//Reposition any scrollbars
	if ((Message==WM_HSCROLL) || (Message==WM_VSCROLL))
	{
		processScrollbarMessage(lParam,wParam);
		return 0;
	}


	//Process hotkey messages
	if (Message==WM_HOTKEY)
	{
		CGadget::events->push(0,(DWORD)Message,(DWORD)wParam,(DWORD)0);
	}

	//Gadget Window Only Handling
	//=====================================

	//Process messages if necessary 
	if (gadget)
	{
		if ((Message == WM_KEYDOWN) && (wParam==VK_TAB) && (!gadget->wantTabs()))
		{
			focusNextGadget(hWnd);

			return 0;
		}

		//Send the message to the event queue so the user can respond to it
		if (isAllowedMessage(Message))
		{
			//Stop processing messages in demo version after timeout
			CGadget::events->push((DWORD)gadget,(DWORD)Message,(DWORD)wParam,(DWORD)lParam);
		}

		//Tree view selection // - Not working at the moment
		/*if ((Message == WM_COMMAND) && (gadget->getType()==GADGET_TREEVIEW))
		{
			if (((LPNMTREEVIEW)(lParam))->hdr.code == TVN_SELCHANGED)
			{
				CGadget::events->push((DWORD)lParam,(DWORD)BGUI_TREEVIEWCHANGE,(DWORD)((LPNMTREEVIEW)(lParam))->itemNew.hItem,0);
			}
		}*/

		if ((Message==WM_CHAR) && (gadget->getType()==GADGET_EDIT))
		{
			//TODO
			//Remove Tabs from the pipeline of single line edit controls

			//Stop annoying beep if enter is pressed
			if ((wParam==VK_RETURN) && (((CGadgetEdit*)gadget)->getMultiLine()==false))
			{
				return 0;
			}

			if ((wParam==VK_TAB) && (((CGadgetEdit*)gadget)->getMultiLine()==false))
			{
				return 0;
			}
		}

		if (Message==WM_PAINT)
		{
			//If auto-update is disabled, throw the paint message away
			if (gadget->getAutoUpdate()==false)
			{
				RedrawWindow(hWnd,0,0,RDW_VALIDATE);
			}
			//Draw canvas gadgets 
			if (gadget->getType()==GADGET_CANVAS)
			{
				((CGadgetCanvas*)(gadget))->drawCanvas();
			}	
		}

		if (Message==WM_LBUTTONUP)
		{
			CGadget::lastClicked=gadget;
		}

		if (Message==WM_MOUSEMOVE)
		{
			if (wParam & MK_LBUTTON) // Left mouse button held down
			{
				gadget->setMouseDown(true);
			}
			else
			{
				gadget->setMouseDown(false);
			}
		}

		if (Message==WM_SIZE)
		{
			gadget->doAlign(); //When the parent window is resized, the gadget is automatically repositioned

			if ((gadget->getType()==GADGET_CANVAS) && (lParam != 0)) //Recreate the memory DC if the canvas is resized
			{
				//debugInt("b",(int)((CGadgetCanvas*)gadget)->getCanvasDC());
				((CGadgetCanvas*)gadget)->initDC(LOWORD(lParam),HIWORD(lParam));
				//debugInt("v",(int)((CGadgetCanvas*)gadget)->getCanvasDC());
			}

		}

		if (Message==WM_ERASEBKGND)
		{
			if (gadget->getManualBackgroundPaint()==true)
			{
					paintBackground(gadget->getHwnd());
			}
		}


		//Don't automatically process WM_CLOSE messages, this allows the user to respond to requests to close
		//the window
		if (Message != WM_CLOSE)
		{
			if (gadget->getOldWinProc())
			{
				return CallWindowProc(gadget->getOldWinProc(), hWnd, Message, wParam,lParam);	
			}
			else
			{
				return DefWindowProc(hWnd,Message,wParam,lParam);
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		//If this is the main application window, call the original window procedure
		if ((CApplication::core) && (hWnd==CApplication::core->hWnd) && (hWnd != 0))
		{
			//if (Message != WM_PAINT)
			//{
				return CallWindowProc((WNDPROC)CApplication::originalMainWindowProc,hWnd,Message,wParam,lParam);
			//}
			//else
			//{
			//	return CallWindowProc((WNDPROC)CApplication::originalMainWindowProc,CApplication::paintWindow,Message,wParam,lParam);
			//}
		}
		else
		{
			return DefWindowProc(hWnd,Message,wParam,lParam);
		}
	}
}

void focusNextGadget(HWND hWnd)
{
			HWND next=GetNextWindow(hWnd,GW_HWNDNEXT);

			if (next)
			{
				SetFocus(next);
			}
			else
			{
				next=GetWindow(hWnd,GW_HWNDFIRST);
				if (next)
				{
					SetFocus(next);
				}
			}
}

CGadget* convertWinHandle(HWND win)
{
	//Retrieve pointer to gadget to which the window belongs
	return (CGadget*)(GetWindowLong(win,GWL_USERDATA));
}

void processScrollbarMessage(LPARAM lParam, WPARAM wParam) 
{
	//lParam contains handle of scrollbar window
	//HIWORD(wParam) contains the position of the thumb

		if (LOWORD(wParam)==SB_THUMBTRACK)
		{
			SendMessage((HWND)lParam,SBM_SETPOS,HIWORD(wParam),TRUE); //Move thumb to mouse position held in wParam
		}

		if (LOWORD(wParam)==SB_LINELEFT)
		{
			int currentPos=SendMessage((HWND)lParam,SBM_GETPOS,0,0);
			SendMessage((HWND)lParam,SBM_SETPOS,currentPos-1,TRUE); //Move thumb one point to the left
		}

		if (LOWORD(wParam)==SB_LINERIGHT)
		{
			int currentPos=SendMessage((HWND)lParam,SBM_GETPOS,0,0);
			SendMessage((HWND)lParam,SBM_SETPOS,currentPos+1,TRUE); //Move thumb one point to the right
		}

		if (LOWORD(wParam)==SB_PAGELEFT)
		{
			int currentPos=SendMessage((HWND)lParam,SBM_GETPOS,0,0);
			int min=0;
			int max=0;
			SendMessage((HWND)lParam,SBM_GETRANGE,(WPARAM)&min,(LPARAM)&max);
			SendMessage((HWND)lParam,SBM_SETPOS,currentPos-(max-min)/10,TRUE); //Move thumb one block to the left
		}

		if (LOWORD(wParam)==SB_PAGERIGHT)
		{
			int currentPos=SendMessage((HWND)lParam,SBM_GETPOS,0,0);
			int min=0;
			int max=0;
			SendMessage((HWND)lParam,SBM_GETRANGE,(WPARAM)&min,(LPARAM)&max);
			SendMessage((HWND)lParam,SBM_SETPOS,currentPos+(max-min)/10,TRUE); //Move thumb one block to the right
		}
}

//Some windows must have their backgrounds manually painted by this function to avoid a nasty
//smear effect - eg CGadgetGroupBox
void paintBackground(HWND hWnd)
{
	//Code from MS KB article Q79982
	//TODO Recode so that I can understand this
	COLORREF gWindowColor;
	gWindowColor=RGB(236,233,216);
	//gWindowColor=RGB(255,0,0);
	HBRUSH  hBrush, hOldBrush;
    HPEN    hPen, hOldPen;
    RECT    rect;
    HDC     hDC;    hDC = GetDC(hWnd);    // Obtain a handle to the parent window's background brush.
    //hBrush = (HBRUSH)GetClassLong(GetParent(hWnd), GCL_HBRBACKGROUND);

	//Selecting a grey brush into the HDC clears the background
	hBrush=CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);    
	
	// Create a background-colored pen to draw the rectangle
    // borders, where gWindowColor is some globally defined
    // COLORREF variable used to paint the window's background
    hPen = CreatePen(PS_SOLID, 1, gWindowColor);
    hOldPen = (HPEN)SelectObject(hDC, hPen);    // Erase the group box's background.
    GetClientRect(hWnd, &rect);

	

    Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);    // Restore the original objects before releasing the DC.

	ExtFloodFill(hDC,5,5,RGB(255,0,0),FLOODFILLBORDER);

    SelectObject(hDC, hOldPen);
    SelectObject(hDC, hOldBrush);    // Delete the created object.
    DeleteObject(hPen);    ReleaseDC(hWnd, hDC);    // Instruct Windows to paint the group box text and frame.
    InvalidateRect(hWnd, NULL, FALSE);    // Insert code here to instruct the contents of the group box
    // to repaint as well.    return TRUE; // Background has been erased.

}

//Sends a message to all windows owned by the specified parent
void _stdcall broadcastMessage(HWND parent, UINT Message, WPARAM wParam, LPARAM lParam)
{
	UINT* msgData	=new UINT[4];
//	char* className	=new char[255];

	*msgData=Message;
	*(msgData+1)=(UINT)wParam;
	*(msgData+2)=(UINT)lParam;
//	*(msgData+3)=(UINT)className; //So we only send message to appropriate classes

	EnumChildWindows(parent,(WNDENUMPROC)broadcastEnumProc,(LPARAM)msgData);

	delete [] msgData;
//	delete [] className;

}

BOOL _stdcall broadcastEnumProc(HWND child, LPARAM userData)
{
	UINT*	msgData	=(UINT*)userData;

	UINT	msg			=*msgData;
	WPARAM	wParam		=(WPARAM)*(msgData+1);
	LPARAM	lParam		=(LPARAM)*(msgData+2);

	//Send message to window
	SendMessage(child,msg,wParam,lParam);

	return TRUE;
}

//Registers the window class for window gadgets
ATOM registerCGadgetWindow()
{

	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX); 


	HBRUSH background=CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)(windowWndProc);
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(0,IDC_ARROW);

	if (CApplication::core)
	{
		wcex.hInstance      = CApplication::core->hInstance;
	}

	//wcex.hbrBackground  = (HBRUSH)(0);
	wcex.hbrBackground	= (HBRUSH)background;
	wcex.lpszMenuName	= (LPCTSTR)0;
	wcex.lpszClassName	= "CGADGETWINDOW";
	wcex.hIconSm		= 0;

	ATOM result=RegisterClassEx(&wcex);
	
	
		if (!result)
		{
			debugInt("registerCGadgetWebBrowser failed",(int)GetLastError());
		}


	return result;
}

ATOM registerCGadgetWebBrowser()
{
	HMODULE webLib=CApplication::webLib;

		lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)webLib, "EmbedBrowserObject");
		lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)webLib, "UnEmbedBrowserObject");
		lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)webLib, "DisplayHTMLPage");
		lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)webLib, "DisplayHTMLStr");
		lpGetCurrentURL = (GetCurrentURLPtr *)GetProcAddress((HINSTANCE)webLib,"GetCurrentURL");
		lpSetSize = (SetSizePtr *)GetProcAddress((HINSTANCE)webLib,"SetSize");

		WNDCLASSEX wc;

		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = CApplication::core->hInstance;
		wc.lpfnWndProc = browserWndProc;
		wc.lpszClassName = "CGADGETWEBBROWSER";

		ATOM result=RegisterClassEx(&wc);

		if (!result)
		{
			debugInt("registerCGadgetWebBrowser failed",(int)GetLastError());
		}

		return result;
}


//Window procedure for Window gadgets
LRESULT CALLBACK windowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK browserWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		// Embed the browser object into our host window. We need do this only
		// once. Note that the browser object will start calling some of our
		// IOleInPlaceFrame and IOleClientSite functions as soon as we start
		// calling browser object functions in EmbedBrowserObject().
		if ((*lpEmbedBrowserObject)(hwnd)) return(-1);

		// Another window created with an embedded browser object
		++WindowCount;

		// Success
		return(0);
	}

	if (uMsg == WM_DESTROY)
	{
		// Detach the browser object from this window, and free resources.
		(*lpUnEmbedBrowserObject)(hwnd);
		
		// One less window
		--WindowCount;

		// If all the windows are now closed, quit this app
		//if (!WindowCount) PostQuitMessage(0);

		return(TRUE);
	}

	// NOTE: If you want to resize the area that the browser object occupies when you
	// resize the window, then handle WM_SIZE and use the IWebBrowser2's put_Width()
	// and put_Height() to give it the new dimensions.

	if (uMsg == WM_SIZE)
	{
		(*lpSetSize)(hwnd,LOWORD(lParam),HIWORD(lParam));

		return (0);
	}

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

//Returns true if the message is permissible, in which case it is passed onto the gadget, or false otherwise
//this cuts down the queue.  If too many messages get through, the "legit" / important ones get chucked off the queue too soon
bool isAllowedMessage(UINT Message)
{
	switch (Message)
	{
	case WM_MOUSEMOVE		: return true;
	case WM_COMMAND			: return true;
	case WM_LBUTTONUP		: return true;
	case WM_RBUTTONUP		: return true;
	case WM_LBUTTONDBLCLK	: return true;
	case WM_RBUTTONDBLCLK	: return true;
	case WM_LBUTTONDOWN		: return true;
	case WM_NCLBUTTONDOWN	: return true;
	case WM_RBUTTONDOWN		: return true;
	case WM_MOVE			: return true;
	case WM_SIZE			: return true;
	case WM_CLOSE			: return true;
	case WM_ACTIVATE		: return true;
	case WM_TIMER			: return true;
	case WM_KEYDOWN			: return true;
	case WM_KEYUP			: return true;
	case WM_MOVING			: return true;
	case WM_SIZING			: return true;
	case WM_KILLFOCUS		: return true;
	case WM_SETFOCUS		: return true;
	case SBM_SETPOS			: return true;
	case WM_MENUSELECT		: return true;

	
	
	default					: return false;
	}
}

/*
DWORD convertColorRGBtoBGR(DWORD color)
{
	return RGB(GetBValue(color),GetGValue(color),GetRValue(color));
}

DWORD convertColorBGRtoRGB(DWORD color)
{
	return RGB(GetBValue(color),GetGValue(color),GetRValue(color));
}
*/

//Colours in DBPro are the opposite order to windows, this means we can use the one function
//to convert either way.
DWORD convertColor(DWORD color)
{
	return RGB(GetBValue(color),GetGValue(color),GetRValue(color));
}

