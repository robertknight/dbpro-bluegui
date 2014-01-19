#include "GUI-DLL.h"

CGadgetFont::CGadgetFont()
{
	myFont=0;
}

bool CGadgetFont::create(char* name, int size, int bold, int italic, int underline)
{
	//From CreateFont documentation:  To convert point size to pixel height
	//nHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	//Obtain the desktop's device context, which provides a reference point for calculating the pixel size equivilent for
	//a given point size.
	HDC deskDC=GetDC(GetDesktopWindow());
	//Use formula from MSDN docs to calc point size
	int height=-MulDiv(size,GetDeviceCaps(deskDC,LOGPIXELSY),72);
	//We no longer need the desktop's DC - get rid of it!
	ReleaseDC(GetDesktopWindow(),deskDC);

	int weight;

	//Toggle bold or normal setting depending on parameters
	if (bold)
	{
		weight=FW_BOLD;
	}
	else
	{
		weight=FW_NORMAL;
	}

	myFont=CreateFont(height,0,0,0,weight,italic,underline,0,
						DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,name);

	if (myFont)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CGadgetFont::create(LOGFONT* fontOptions)
{
	myFont=CreateFontIndirect(fontOptions);

	if (myFont)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGadgetFont::apply(CGadget* gadget)
{
	

	if (gadget->getType()==GADGET_CANVAS)
	{
		((CGadgetCanvas*)(gadget))->setCanvasFont(this);
		
	}
	else
	{
		SendMessage(gadget->getHwnd(),WM_SETFONT,(WPARAM)myFont,MAKEWORD(TRUE,0));
	}
}

void CGadgetFont::destroy()
{
	DeleteObject(myFont);
	myFont=0;
}

HFONT CGadgetFont::getHandle()
{
	return myFont;
}

char* CGadgetFont::getFace()
{
	LOGFONT info;
	char*   fontName=new char[32];

	getFontInfo(&info);

	strcpy(fontName,(char*)&info.lfFaceName);

	

	return fontName;
}

int CGadgetFont::getSize()
{
	float size;
    HDC screenDC;
	int pixelsPerInch;
	LOGFONT info;

	getFontInfo(&info);

	screenDC=GetDC(GetDesktopWindow());
	pixelsPerInch=GetDeviceCaps(screenDC,LOGPIXELSY);
	ReleaseDC(GetDesktopWindow(),screenDC);

    size=(float)((72.0*abs(info.lfHeight))/pixelsPerInch);
     
	//Counter flooring effect of int division
	int sizeInt=(int)(size+0.5);


	return sizeInt;
}

int CGadgetFont::isBold()
{
	LOGFONT info;

	getFontInfo(&info);

	if (info.lfWeight > FW_NORMAL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CGadgetFont::isItalic()
{
	LOGFONT info;

	getFontInfo(&info);

    return info.lfItalic;
}

int CGadgetFont::isUnderlined()
{
	LOGFONT info;

	getFontInfo(&info);

	return info.lfUnderline;
}

void CGadgetFont::getFontInfo(LOGFONT* info)
{
	GetObject((HGDIOBJ)myFont,sizeof(LOGFONT),info);
}

