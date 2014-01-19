
class CGadgetMenu 
{
public:
	CGadgetMenu();

	//Create a new menu
	void		create();

	//Destroy the menu
	void		destroy();

	//Create a new popup menu
	void		createPopup();
	
	//Add the new menu to a window
	void		attachToWindow(HWND window);
	
	//Add the new menu to an existing menu item
	void		attachToMenu(HMENU menu, int item);

	//Show the menu as a popup
	int			showPopup(int x, int y);  

	//Add a new bitmap to the menu's image list
	void		addImage(char* fileName);

	//Add a new item to the menu
	void		addItem(char* text, int returnValue, int insertBefore=-1, char* menuIcon=0);

	//Set the checked/unchecked state of an item
	void		setItemChecked(int item, int boolChecked);

	//Set checked/unchecked state of radio item
	void		setItemRadioChecked(int item, int firstGroupItem, int lastGroupItem);

	//Set the enabled/disabled state of an item
	void		setItemEnabled(int item, int boolEnabled);

	//Adds a splitter item to the menu
	void		addSplitter();

	//Set text of a menu item
	void		setItemText(int item, char* caption);

	//--Expressions--

	//Get checked/unchecked state of an item
	int			getItemChecked(int item);

	//Get enabled/disabled state of an item
	int			getItemEnabled(int item);

	//Get text of a menu item
	void		getItemText(int item, char* outCaption, int maxLength);

	//Get number of items in a menu
	int			getItemCount();

	//--Accessor Methods--

	//Return menu handle
	HMENU		getHMenu() {return myMenu;}

private:
	//Handle to the window menu
	HMENU		myMenu;

	//Handle to the parent menu
	HMENU		myParentMenu;

	//Identifier of the parent menu item
	int			myParentIdentifier;

	//Handle of the window associated with the menu
	HWND		myWindow;

	//Number of images in the menu
	DWORD		myImageCount;

	//Pointer to the menu images
	HBITMAP*	myImages;

	
};

