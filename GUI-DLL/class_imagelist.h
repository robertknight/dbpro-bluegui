#ifndef IMAGELIST_H
#define IMAGELIST_H

class CGadgetImageList
{
public:
	void		create(int width, int height, int maxSize);

	int			addImage(char* fileName);
	int			addImage(int imageIndex);
	void		removeImage(int index);

	HIMAGELIST	getHImageList();
	
private:
	HBITMAP		myLoadImageFromMem(int imageIndex);
	HBITMAP		myCreateMaskBmp(HBITMAP source, bool disableTransparency=false);
	HIMAGELIST	myImages;
	DWORD		myWidth;
	DWORD		myHeight;
};

#endif