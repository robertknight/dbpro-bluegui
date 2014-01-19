// KeyGen.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h>
#include "KeyGen.h"
#include "commctrl.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND				editUser;
HWND				editResult;
HWND				btnGen;
HWND				mainWin;

char				buffer[255];
char				result[255];


void				Crypt(TCHAR* imp, DWORD inplen, TCHAR *key="",DWORD keylen=0);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	ZeroMemory(&buffer,255);

	InitCommonControls();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KEYGEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_KEYGEN);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if ((msg.hwnd==btnGen) && (msg.message==WM_LBUTTONUP))
		{

			ZeroMemory(&result,255);

			SendMessage(editUser,WM_GETTEXT,255,(LPARAM)(char*)&buffer);

			Crypt((char*)&buffer,(WPARAM)strlen((char*)&buffer),"B2ENCSTR",(LPARAM)strlen("B2ENCSTR"));

			
			

			int len=strlen((char*)&buffer);

			int chr=0;

			char out[20];

			for (int i=0;i<len;i++)
			{
				int chr=*(buffer+i);

				sprintf((char*)out,"%x",chr);

				strcat((char*)result,(char*)out);
			}

			SendMessage(editResult,WM_SETTEXT,0,(LPARAM)&result);
		}

		
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				
				
					//if (IsDialogMessage(GetParent(msg.hwnd),&msg))
					{
					//	TranslateMessage(&msg);

						//if (msg.hwnd==mainWin)
						{
					//		printf("Done");
						}

					}
				//	else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				

				
			}
		
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_KEYGEN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_KEYGEN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 200, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   mainWin=hWnd;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HWND st=0;

	switch (message) 
	{
	case WM_CREATE:
		st=CreateWindow("STATIC",0,WS_VISIBLE|WS_CHILD,0,0,300,300,hWnd,0,0,0);
		editUser=CreateWindowEx(0,"EDIT",0,WS_TABSTOP|WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL,20,20,75,25,st,0,0,0);
		editResult=CreateWindowEx(0,"EDIT",0,WS_TABSTOP|WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL,20,50,75,25,st,0,0,0);
		btnGen=CreateWindowEx(0,"BUTTON","Generate Key",WS_TABSTOP|WS_VISIBLE|WS_CHILD,20,80,100,25,st,0,0,0);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Crypt(TCHAR *inp, DWORD inplen, TCHAR* key, DWORD keylen)
{
    //we will consider size of sbox 256 bytes
    //(extra byte are only to prevent any mishep just in case)
    TCHAR Sbox[257], Sbox2[257];
    unsigned long i, j, t, x;

    //this unsecured key is to be used only when there is no input key from user
    static const TCHAR  OurUnSecuredKey[] = "www.systweak.com" ;
    static const int OurKeyLen = _tcslen(OurUnSecuredKey);    
    TCHAR temp , k;
    i = j = k = t =  x = 0;
    temp = 0;

    //always initialize the arrays with zero
    ZeroMemory(Sbox, sizeof(Sbox));
    ZeroMemory(Sbox2, sizeof(Sbox2));

    //initialize sbox i
    for(i = 0; i < 256U; i++)
    {
        Sbox[i] = (TCHAR)i;
    }

    j = 0;
    //whether user has sent any inpur key
    if(keylen)
    {
        //initialize the sbox2 with user key
        for(i = 0; i < 256U ; i++)
        {
            if(j == keylen)
            {
                j = 0;
            }
            Sbox2[i] = key[j++];
        }    
    }
    else
    {
        //initialize the sbox2 with our key
        for(i = 0; i < 256U ; i++)
        {
            if(j == OurKeyLen)
            {
                j = 0;
            }
            Sbox2[i] = OurUnSecuredKey[j++];
        }
    }

    j = 0 ; //Initialize j
    //scramble sbox1 with sbox2
    for(i = 0; i < 256; i++)
    {
        j = (j + (unsigned long) Sbox[i] + (unsigned long) Sbox2[i]) % 256U ;
        temp =  Sbox[i];                    
        Sbox[i] = Sbox[j];
        Sbox[j] =  temp;
    }

    i = j = 0;
    for(x = 0; x < inplen; x++)
    {
        //increment i
        i = (i + 1U) % 256U;
        //increment j
        j = (j + (unsigned long) Sbox[i]) % 256U;

        //Scramble SBox #1 further so encryption routine will
        //will repeat itself at great interval
        temp = Sbox[i];
        Sbox[i] = Sbox[j] ;
        Sbox[j] = temp;

        //Get ready to create pseudo random  byte for encryption key
        t = ((unsigned long) Sbox[i] + (unsigned long) Sbox[j]) %  256U ;

        //get the random byte
        k = Sbox[t];

        //xor with the data and done
        inp[x] = (inp[x] ^ k);
    }    
}
