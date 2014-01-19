// GUI-DLL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GUI-DLL.h"
#include <commctrl.h>


#define MAX_LOADSTRING 100

//DLL Export Macros
#ifdef DBPRO_DLL
	#define EXPORT extern "C" __declspec (dllexport)
#else
	#define EXPORT extern "C" __stdcall (dllexport)
#endif
#define EXPORT_MANGLED __declspec (dllexport)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

//Make static members of CGadget available to winMain
HINSTANCE			CGadget::hInst;
CGadget*			CGadget::lastClicked;
CGadgetEvents*		CGadget::events;

//RC4-style encryption/decryption algorithm

void Crypt(TCHAR *inp, DWORD inplen, TCHAR* key, DWORD keylen)
{
    //we will consider size of sbox 256 bytes
    //(extra byte are only to prevent any mishep just in case)
    TCHAR Sbox[257], Sbox2[257];
    unsigned int i, j, t, x;

    //this unsecured key is to be used only when there is no input key from user
    static const TCHAR  OurUnSecuredKey[] = "BLU.PLUGINV2.KEY" ;
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



//This function readies the application to use the common controls DLL 
//so that we can use gadget classes from it.
void GSetupCommonControls()
{
	INITCOMMONCONTROLSEX commonControlClass;
	
	commonControlClass.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//updown control is class, bar control include
	//status bar, tooltip,trackbar, toolbar is classes
	
	commonControlClass.dwICC =	ICC_COOL_CLASSES|ICC_UPDOWN_CLASS|ICC_BAR_CLASSES|ICC_TAB_CLASSES|
								ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES|ICC_PROGRESS_CLASS; 
	
	InitCommonControlsEx(&commonControlClass);
}








