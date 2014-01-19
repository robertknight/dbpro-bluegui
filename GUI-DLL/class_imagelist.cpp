

#include "GUI-DLL.h"

void CGadgetImageList::create(int width, int height, int maxSize)
{
	myImages=ImageList_Create(width, height, ILC_COLOR32|ILC_MASK, 1, maxSize);
	myWidth=width;
	myHeight=height;
}


int CGadgetImageList::addImage(char* fileName)
{
	HBITMAP newImage=(HBITMAP)LoadImage(NULL,fileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	HBITMAP newImageMsk=myCreateMaskBmp(newImage);
//	HBITMAP newImageMsk=0;
	int result = ImageList_Add(myImages,newImage,newImageMsk);

	DeleteObject(newImage);
	DeleteObject(newImageMsk);

	return result;
}

int CGadgetImageList::addImage(int imageIndex)
{
	HBITMAP newImage=myLoadImageFromMem(imageIndex);
	HBITMAP newImageMsk=myCreateMaskBmp(newImage,true);

	int result=ImageList_Add(myImages,newImage,newImageMsk);
	
	DeleteObject(newImage);

	return result;
}

void CGadgetImageList::removeImage(int index)
{
	ImageList_Remove(myImages, index);
}

HIMAGELIST CGadgetImageList::getHImageList()
{
	return myImages;
}

HBITMAP CGadgetImageList::myCreateMaskBmp(HBITMAP source,bool disableTransparency)
{
	//Create a bitmap and device context compatible with the main window
#ifdef DBPRO_DLL
	HDC coreDC = GetDC(CApplication::core->hWnd);
#else
	HDC coreDC=GetDC(GetDesktopWindow());
#endif
	HDC			maskDC=CreateCompatibleDC(coreDC);
	HDC			sourceDC=CreateCompatibleDC(coreDC);
	HBITMAP		newBmp=CreateCompatibleBitmap(maskDC,myWidth,myHeight);
#ifdef DBPRO_DLL
	ReleaseDC(CApplication::core->hWnd,coreDC);
#endif

	

	//Select our original bitmap into the new device context
	HBITMAP originalSrcBmp=(HBITMAP)SelectObject(sourceDC,source);
//	RealizePalette(sourceDC);

	//Select mask bitmap into the new device (mask) context
	HBITMAP originalMskBmp=(HBITMAP)SelectObject(maskDC,newBmp);

	//Read colour of transparent pixel (use colour of top left hand pixel)
	DWORD maskColour=GetPixel(sourceDC,0,0);

	//Go through each pixel in the image and set the mask pixel to white if the colour matches
	//the transparent colour.

	for (DWORD y=0; y<myHeight; y++)
	{
		for (DWORD x=0; x<myWidth; x++)
		{
			if ((GetPixel(sourceDC,x,y)==maskColour) && (!disableTransparency))
			{
				SetPixelV(maskDC,x,y,RGB(0xFF,0xFF,0xFF));
			}
			else
			{
				SetPixelV(maskDC,x,y,RGB(0,0,0));
			}
		}
	}

	//Select new bitmaps out of DCs
	SelectObject(sourceDC,originalSrcBmp);
	SelectObject(maskDC,originalMskBmp);

	//Delete device contexts
	DeleteDC(maskDC);
	DeleteDC(sourceDC);

	//Return handle to mask bitmap
	return newBmp;
}

HBITMAP CGadgetImageList::myLoadImageFromMem(int imageIndex)
{
	//TODO - Unify code to avoid copying

	//Get DBPro core data ptr
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
		return 0;
	}
	
	//Function pointers valid - do clever stuff :)

	//First check that the image exists
	if (!imageExists(imageIndex))
	{
		return 0;
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
		return 0;
	}

	//Create a memblock from the image
	makeMemblockFromImage(memblockId,imageIndex);

	//Get a pointer to the memblock
	DWORD* memblockPtr=(DWORD*)getMemblockPtr(memblockId);

	//Get the size, width & depth of the image
	//Note memblock structure:  WIDTH [4 Bytes], HEIGHT [4 Bytes], DEPTH [4 Bytes], PIXEL DATA [Width*Height*(Depth/8) Bytes]
	DWORD imageWidth	= *(memblockPtr);
	DWORD imageHeight	= *(memblockPtr+1);
	DWORD imageDepth	= *(memblockPtr+2);
	DWORD pixelDataSize	= imageWidth*imageHeight*(imageDepth/8);
	DWORD offset		= 3; //Set offset to first DWORD past 3-DWORD header

	//Create a bitmap and device context compatible with the main window
#ifdef DBPRO_DLL
	HDC coreDC = GetDC(CApplication::core->hWnd);
#else
	HDC coreDC = GetDC(GetDesktopWindow());
#endif
	HBITMAP		newBmp=CreateCompatibleBitmap(coreDC,imageWidth,imageHeight);
	HDC			newDC=CreateCompatibleDC(coreDC);
#ifdef DBPRO_DLL
	ReleaseDC(CApplication::core->hWnd,coreDC);
#endif
	//Select bitmap into new DC
	HBITMAP originalBmp=(HBITMAP)SelectObject(newDC,newBmp);

	//Write pixel data
	DWORD dbproColorValue	=0;

	//Copy the data pixel-by-pixel, we can't use BitBlt or a similar function because
	//the colour components have to be re-arranged
	for (DWORD y=0;y<imageHeight;y++)
	{
		for (DWORD x=0;x<imageWidth;x++)
		{
			//Retrieve next pixel's colour value
			dbproColorValue=*(memblockPtr+offset);
			//Write pixel to DC (and therefore bitmap) - rearrange pixel elements first
			SetPixelV(newDC,x,y,convertColor(dbproColorValue));
			//Increment offset to we read from next pixel
			offset++;
		}
	}

	//Delete the memblock
	deleteMemblock(memblockId);

	//Select bitmap out of DC
	SelectObject(newDC,newBmp);
	//Delete the memory DC
	DeleteDC(newDC);

	return newBmp;
}


