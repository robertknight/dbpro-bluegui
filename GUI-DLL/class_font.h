class CGadgetFont
{
public:
	CGadgetFont();

	//Creates the new font
	bool  create(char* name, int size, int bold, int italic, int underline);
	bool  create(LOGFONT* fontOptions);
	
	//Destroys the font
	void  destroy();

	//Applies a font to a gadget
	void  apply(CGadget* gadget);

	//Get font face
	char* getFace();

	//Get font size
	int   getSize();

	//Get bold flag
	int   isBold();

	//Get italic flag
	int   isItalic();

	//Get underline flag
	int   isUnderlined();

	//Get font handle
	HFONT  getHandle();

private:
	//Gets font information into a LOGFONT structure
	void  getFontInfo(LOGFONT* info);

	//Stores the handle of the Windows font
	HFONT myFont;
};