#ifndef GADGET_H
#define GADGET_H

//--Includes required for Gadget classes--
#include "debug.h"
#include "class_gadgetevent.h"
#include <list>
//#include "class_font.h"

class CGadgetFont;

//--Constant Definitions--

//Bare bones window styles necessary to get a gadget to display, WS_CHILD to ensure that the window behaves correctly
//as a child of a window, WS_VISIBLE to set it initially visible upon creation and WS_CLIPCHILDREN so that other gadgets
//can be parented to it
#define DEFAULT_GADGET_STYLES WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS

//--Custom Events
enum BLUEGUI_CUSTOM_EVENTS
{
	BCE_BASE=WM_USER+100,
	
	BCE_BUTTON_CLICK
};

//--Enumerations for CGadget and derived classes--
enum PANEL_BORDER_STYLE
{
	PB_NONE,
	PB_FLATBLACK,
	PB_3D_RAISED,
	PB_3D_SUNKEN
};

enum ALIGN_TYPE
{
	ALIGN_NONE,
	ALIGN_ALL,
	ALIGN_TOP,
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_BOTTOM
};

enum TEXT_ALIGN
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

enum GADGET_TYPE
{
	GADGET_NONEXISTANT,
	GADGET_BUTTON,
	GADGET_EDIT,
	GADGET_STATIC,
	GADGET_LISTBOX,
	GADGET_COMBOBOX,
	GADGET_TOOLBAR,
	GADGET_STATUSBAR,
	GADGET_SCROLLBAR,
	GADGET_CANVAS,
	GADGET_CHECKBOX,
	GADGET_RADIOBUTTON,
	GADGET_TREEVIEW
};

//========================================================
//All gadget creation functions follow the same format:

//-Create the window with the appropriate class and style flags
//-Return if window creation failed
//-Otherwise:
//	-Subclass window
//	-Apply default properties
//	-Return true



// Top-level Gadget Class
class CGadget
{
public:

	//Constructor
	CGadget();

	//--Commands--//

	//Registers the class for this window using myWindowClass
	ATOM				registerClass(HINSTANCE hInstance);

	//Creates the gadget window
	virtual bool		create();

	//Destroy gadget window
	virtual void		destroy();

	void enableXPStyle(bool xpStyle);

	//Creates a gadget from an existing window
	bool				createFromWindow(HWND existingWindow);

	//Sets the default properties of the gadget window class
	//structure (myWindowClass)
	//virtual void		setClassDefaults();

	//Sets the text of the gadget's window
	virtual void		setText(char* newCaption);

	//Sets the alignment of the gadget's text (not supported by all gadgets)
	virtual void		alignText(TEXT_ALIGN textAlignment);

	//Used after to creation to give gadgets a normal font rather than the ugly
	//bold windows default.
	void				setDefaultFont();

	//Adjusts the position and dimensions of the gadget's window
	virtual void		move(int xPos, int yPos, int width, int height, bool clientSize=false);

	//Hides or shows the gadget window depending on the flag
	virtual void		setVisible(DWORD flag, BOOL centerInParent=0);

	//Enables/disables user input to the gadget
	void				setEnabled(DWORD flag);

	//Tells the gadget to repaint itself to reflect any visual changes
	void				update();

	//Sets the colour of a specific pixel in a gadget window's client area
	virtual void		drawPoint(DWORD x, DWORD y, COLORREF color);

	//Retrieves events for all gadgets - can wait for event to occur
	static
	bool				getEvents(bool waitForEvent=true);

	//Sends a message to a gadget (wrapper for SendMessage)
	LRESULT				sendMsg(UINT message, WPARAM wParam, LPARAM lParam);

	//Loads an image into a gadget from a bitmap file
	bool				loadImageFromFile(char* fileName);

	//Loads an image into a gadget from a DBPro image
	bool				loadImageFromMem(int imageIndex);

	//Causes the gadget to align itself to its parent window according to its
	//auto-align property
	void				doAlign();

	//Makes a gadget active for keyboard input
	void				makeActive();

	//INCOMPLETE //Process gadget resizing
	void				doDragResize();

	//Sets the gadget to send out a message after a certain amount of time
	void				setTimer(DWORD milliseconds);


	//Sets a gadget's parent window
	void				setParent(HWND parentWindow);

	//Sets the custom drawing colours for the gadget
	virtual
	void				setColors(COLORREF background, COLORREF text);

	//Sets the shape of the gadget
	void				setShape(char* fileName);

	//Sets the opacity of the gadget
	virtual
	void				setAlpha(int alpha);

	//Sets whether or not the gadget receives tabs
	void				setTabFocus(bool tabFocus);

	//Used for drawing gadget with custom colours 
	virtual
	DWORD				customPaint(HDC deviceContext);

	void				setTabOrder(CGadget* insertAfter);

	virtual bool		commandHandler(unsigned short commandCode) {return false;}

	//--Expressions--//
	
	//Does this gadget swallow tabs?
	virtual bool		wantTabs();

	virtual void		handleNotification(LPNMHDR event);
	void				sendNotification(int eventCode);

	//Retrieves information about the gadget's size
	int					getWidth();
	int					getHeight();
	int					getClientWidth();
	int					getClientHeight();

	//Retrieves information about the gadget's position
	int					getX(bool returnAbsolute=false, bool returnClient=false);
	int					getY(bool returnAbsolute=false, bool returnClient=false);

	//Retrieves a gadget's title bar text or the text inside the gadget for non-window gadget controls
	char*				getText();

	//Retrieves whether the gadget is hidden or visible
	int					getVisible();

	//Brings the window to the top of the Z order relative to other windows with the same parent
	void				bringToFront();

	//Sends the window to the bottom of the Z order relative to other windows with the same parent
	void				sendToBack();

	//Retrieves whether the gadget has user input enabled or disabled
	DWORD				getEnabled();

	//--Accessor Methods--//

	//Sets the user-defined data associated with the gadget
	void				setData(DWORD data) {myData=data;}

	//Retrieves the user-defined data associated with the gadget
	DWORD				getData() {return myData;}

	//Returns the windows handle for a gadget
	HWND				getHwnd() const {return myHwnd;}

	//Returns bitmap handle for gadget
	HANDLE				getBitmap() const {return gadgetImage;}

	//Returns the auto-align mode for the gadget
	ALIGN_TYPE			getAlign() const {return align;}

	//Sets the auto-align mode for a gadget
	void				setAlign(ALIGN_TYPE alignMode) {align=alignMode; sendMsg(WM_SIZE,0,0);} //Force gadget to implement align

	//Enables / disables auto-update
	void				setAutoUpdate(bool autoUpdate);

	//Retrieves auto-update setting
	bool				getAutoUpdate() {return !disableAutoUpdate;}

	//Sets the window handle for a gadget
	void				setHwnd(HWND newHwnd) {myHwnd=newHwnd;}

	//Retrieves original window procedure handle for drawing of gadget
	WNDPROC				getOldWinProc(){return myOldWinProc;};

	//Sets whether or not the gadget's background must be manually painted
	void				setManualBackgroundPaint(bool manual) {requiresManualBackgroundPaint=manual;}

	//Gets whether or not the gadget's background must be manually painted
	bool				getManualBackgroundPaint() {return requiresManualBackgroundPaint;}

	//Sets the gadget's type value
	void				setType(GADGET_TYPE gadgetType) {myGadgetType=gadgetType;}

	//Retrieves the gadget's type value
	GADGET_TYPE			getType() {return myGadgetType;}

	//Set drag resize settings
	void				setDragResizing(int dragResizeSide) {myDragResizeSide=dragResizeSide;}

	

	//Get drag resize setting
	int					getDragResizing() {return myDragResizeSide;}

	//Set mouse down status
	void				setMouseDown(bool mouseDown) {myMouseDown=mouseDown;}

	//Get mouse down status
	bool				getMouseDown() {return myMouseDown;}

	//Stores the application instance handle needed when creating new gadgets
	static
	HINSTANCE			hInst;

	//Stores a pointer to the last gadget clicked on for easy access
	static
	CGadget*			lastClicked;

	//Pointer to CGadgetEvents class containing details of the last event happening
	//to a gadget

	static
	CGadgetEvents*		events;

	//Default gadget styles flag
	static
	DWORD			defaultStyles;

	//Apply the image to the gadget 
	virtual void	applyImage();

protected:
	//--Functions--
	HRGN bitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);

	virtual void onSize(int width , int height) {};

	//--Variables--

	//Stores the windows handle for the gadget provided by CreateWindow()
	HWND			myHwnd;
	
	//A pointer to the original window proc for use when the Gadget is subclassed so its messages can be
	//processsed
	WNDPROC			myOldWinProc;
	
	//Stores information about the type of the window, the window
	//procedure, window properties etc.
	//WNDCLASSEX		myWindowClass;

	//Handle to gadget's image
	HBITMAP			gadgetImage;

	//Handle to the gadget's alternate HDC
	HDC				gadgetDC;

	//Sets whether or not the gadget automatically updates itself
	bool			disableAutoUpdate;

	//User-defined gadget data
	DWORD			myData;

	//Additional user-defined data
	DWORD			myDataEx;

	//Gadget's auto-align style
	ALIGN_TYPE		align;
	
	//Some gadgets, eg GroupBoxes have wrongly painted backgrounds if their parent has the WS_CLIPCHILDREN propery
	//because the parent can't paint the background properly.  Enabling this flag causes a forced repaint
	bool			requiresManualBackgroundPaint;

	//Stores a value specifying the type of the gadget, for use when one function has to deal with multiple gadget
	//types
	GADGET_TYPE		myGadgetType;
	
	//Initially false, set to true when the user wants the gadget to be destroyed when it receives a WM_CLOSE message
	bool			myCanClose;

	//Drag-resizing variables
	int				myDragResizeSide; //0=Off, 1=Left, 2=Right

	//Mouse status
	bool			myMouseDown; //true when mouse is clicked on gadget

	//Custom Painting information
	HBRUSH			backgroundBrush;
	COLORREF		textColor;


	
};

/*
//Slider Base Classes & Item List Base Classes Not Yet Used
//=========================================================

//Slider Base Gadget - base class for all gadgets which have range & position properties 
class CGadgetSliderBase : public CGadget
{
public:

	//Set the range of the slider gadget
	virtual void setRange(int minV, int maxV);

	//Set the position of the slider gadget
	virtual void setPosition(int position);

	//Retrieve the position of the slider gadget
	virtual int getPosition();

	//Get the minimum value of the slider gadget
	virtual int getMinValue();

	//Get the maximum value of the slider gadget
	virtual int getMaxValue();
};

//Item List Base Gadget - base class for all gadgets which store items of data
class CGadgetItemListBase : public CGadget
{
public:
	
	//Add a new item to the list
	virtual void addItem(char* caption, int image=-1, int insertBefore=-1);

	//Change the text of an item in the list
	virtual void changeItem(int index, char* newCaption);

	//Remove an item from the list
	virtual void removeItem(int index);

	//Count the number of items in the list
	virtual int  itemCount();

	//Retrieve the text of an item
	virtual void itemText(int index);

	//Associate an image list with the gadget
	virtual void setImageList(CGadgetImageList* imageList);

}

//===================================================== */

//Standard button type gadget
class CGadgetButton: public CGadget
{
	public:
		//Creates the button gadget
		bool create(int x, int y, int width, int height, char* caption, bool flat,HWND parent);
		void applyImage();	

		virtual bool		commandHandler(unsigned short commandCode);
		virtual void		handleNotification(LPNMHDR event);

		static CGadgetButton* BoldFocusButton;
	
	protected:
		void _setBoldFocus(bool boldFocus);

		
		
};



//Checkbox-type button gadget
class CGadgetCheckbox: public CGadget
{
public:
	//Creates a checkbox gadget - used to indicate the user's preferences for an operation
	bool create(int x, int y, int width, int height, char* caption, HWND parent);


	//--Commands--//

	//Checks or unchecks the gadget (display a tick in the checkbox)
	void setChecked(bool checked);

	//--Expressions-//

	//Find out whether or not a checkbox is checked.
	bool getChecked();
};

//Radiobutton-Type button gadget
class CGadgetRadiobutton: public CGadget
{
public:
	//Creates a radiobutton gadget - allows user to pick from a range of options
	bool create(int x, int y, int width, int height, char* caption, HWND parent);

	//--Commands--//

	//Selects or unselects a radiobutton
	void setChecked(bool checked);

	//--Expressions--//

	//Find out whether or not a radiobutton is selected.
	bool getChecked();
};

//Frame gadget used as a container for others
class CGadgetFrame: public CGadget
{
public:
	//Creates a frame gadget - this is a captioned box used to contain other gadgets
	bool create(int x, int y, int width, int height, char* caption, HWND parent);
};

//Standard Edit Gadget
class CGadgetEdit: public CGadget
{
public:
		//Create an edit gadget - this is a simple text entry gadget
		bool	create(int x, int y, int width, int height, int settings,HWND parent);

		//Override - Aligns gadget text
		void	alignText(TEXT_ALIGN textAlignment);

		//Limits the amount of text the user can enter
		void	setMaxTextLength(int length);

	//Specific edit gadget methods

	//Commands

		//Sets the selected text in the edit gadget
		void	setSel(int start, int len);

		//Enables / disables editing of the control
		void	setReadOnly(bool readOnly);



		//Replaces selected text
		void	setSelText(char* text);

	//Expressions

		//Retrieves the position of the first selected character
		int		getSelStart();
		//Retrieves the length of the selected text
		int		getSelLen();

		//Retrieves read-only state of control
		bool	getReadOnly();

		//Retrieves multi-line state of control
		bool	getMultiLine() const {return isMultiLine;}

		virtual
		bool	wantTabs();

private:
		bool	isReadOnly;
		bool	isMultiLine;

};


//Static Gadget - Used for labels, panels etc.
class CGadgetStatic: public CGadget
{
public:
	CGadgetStatic() {isTransparent=false;}

	bool	create(int x, int y, int width, int height, char* caption, HWND parent);

	virtual void setText(char* newCaption);

	void	setTransparent(bool transparent);

	//Sets whether the static gadget has a flatborder,no border or 3D border
	void	setBorderStyle(PANEL_BORDER_STYLE borderStyle);

	//Applies the loaded image to the gadget
	void    applyImage();

	//Override - aligns the text in the gadget
	void	alignText(TEXT_ALIGN textAlignment);

	virtual DWORD customPaint(HDC deviceContext);
protected:
	bool isTransparent;
};

//Listbox gadget - a simple list of strings displayed in a scrollable box from which the user can
//select an item
class CGadgetListbox: public CGadget
{
public:
	//Create a listbox gadget
	bool	create(int x, int y, int width, int height, HWND parent);

	//--Commands--

	//Add a new string item to a listbox
	void	addItem(char* itemText, int index=-1);

	//Remove a string item from a listbox
	void	removeItem(int index);

	//Change a string item in a listbox
	void	changeItem(int index,char* newCaption);

	//Delete all items from a listbox
	void	clear();

	//Set the top index of the listbox - so users can scroll down, eg in a chat window
	void	setTopIndex(int index);

	//Select an item in a listbox
	void	selectItem(int index);

	//--Expressions--
	
	//Retrieve zero-based index of currently selected item
	int		selectedItem();

	//Retrieve text for specified item
	char*	getItemText(int index);

	//Retrieve the number of items in a list
	DWORD	getItemCount();

};

//Scrollbar Gadget - an area with a dragable bar in the center used to scroll the contents of other gadgets
class CGadgetScrollbar: public CGadget
{
public:
	//Creates the scrollbar gadget
	bool create(int x, int y, int width, int height, int style, HWND parent); //for style param, 0=Horizontal, 1=Vertical

	//--Commands--

	//Sets the minimum and maximum values of the scrollbar's thumb
	void setRange(int minValue, int maxValue);

	//Sets the position of the thumb in the shaft
	void setPosition(int position);

	//Sets the size of the thumb in the shaft
	void setThumbSize(int size);

	//--Expressions--

	//Retrieves the minimum value of the thumb range
	int getMinimumValue();

	//Retrieves the maximum value of the thumb range
	int getMaximumValue();

	//Retrieves the current position of the scrollbar's thumb
	int getPosition();

	//Retrieves the current thumb size
	int getThumbSize();
};

//Combobox Gadget - similar to a listbox except the list is dropped down when a button is clicked, allowing the list
//to be hidden with only the selected item visible in a static box when not in use
class CGadgetCombobox: public CGadget
{
public:
	//Create a Combobox gadget
	bool create(int x, int y, int width, int height, HWND parent);

	//--Commands--

	//Add a new string item to a combobox
	void addItem(char* itemText, int index=-1);

	//Remove a string item from a combobox
	void removeItem(int index);

	//Change the text of a string item in a combobox
	void changeItem(int index, char* newCaption);

	//Delete all items from a combobox
	void clear();
	
	//Select an item in a combobox
	void selectItem(int index);

	
	virtual bool commandHandler(unsigned short commandCode);

	//--Expressions--

	//Retrieve zero-based index of currently selected item
	int selectedItem();

	//Retrieve text for item
	char* getItemText(int index);

	//Retrieve number of items in list
	DWORD getItemCount();

private:
	//Index of currently selected item
	int currentSel;
	
};

struct TabAssociation
{
	int			tab;
	CGadget*	gadget;
};

//Tabs gadget - allows programmer to split a dialog or toolwindow into manageable areas accessed via tabs at the
//top of the tabs gadget
class CGadgetTabs: public CGadget
{
public:
	//Create the tabs gadget
	bool	create(int x, int y, int width, int height, int style, HWND parent);

	void	handleNotification(LPNMHDR event);

	//--Commands--

	//Add a tab to the gadget
	void	addTab(char* caption, int index=-1);

	//Remove a tab from the gadget
	void	removeTab(int index);

	//Change a tab's text
	void	changeTab(int index, char* newCaption);

	//Set currently selected tab
	void	setTab(int index);

	//--Expressions--

	//Retrieve currently selected tab
	int		selectedTab();

	//Retrieve tab count
	int		getTabCount();

	//Retrieve text of specified tab
	char*	getTabCaption(int index);

	//Retrieves the display rectangle for the tab gadget
	void	getDisplayRect(RECT* displayRect);

	void	attachGadget(int tab, CGadget* gadget);
	void    detatchGadget(int tab, CGadget* gadget);

private:
	std::list<TabAssociation> _attachedGadgets;
	
};

//Statusbar gadget - horizontal strip at the bottom of a window used to display various status info.
//IMPORTANT NOTE - The parent window's window procedure must be SUBCLASSED so that it sends the WM_SIZE message
//to it whenever it receives a WM_SIZE message itself.
class CGadgetStatusbar: public CGadget
{
public:
	//Create the statusbar gadget
	bool	create(HWND parent);

	//--Commands--

	//Add a panel to the statusbar gadget
	void	addPanel(DWORD width,bool enableAutoSize);	//If enableAutoSize is true then the 
													//panel automatically adjusts its width when its text is changed.

	//Remove a panel from the statusbar gadget
	void	removePanel(DWORD index);

	//Set a statusbar panel's text
	void	setPanelText(int index, char* caption);

	//Set a statusbar panel's width
	void	setPanelWidth(DWORD index, DWORD width);

	//--Expressions--

	//Get the number of panels in the status bar
	int		getPanelCount();

	//Get the text of a panel
	char*	getPanelText(int index);


protected:
	//Array storing automatic sizing / manual panel sizing attribute for all panels in gadget
	bool* autoSize;

	//Store the number of parts in a statusbar 

};

//A toolbar usually displayed at the top of the program window providing quick access to key
//features of the application via a series of icon buttons 
class CGadgetToolbar: public CGadget
{
public:
	void destroy();
	//Create the toolbar gadget
	bool create(HWND parent);
	
	virtual void setVisible(DWORD flag, BOOL centerInParent=0);	

	//--Commands--

	//Add a button to the toolbar
	void	addButton(int index,bool makeCheckButton, char* tooltipText);  //If makeCheckButton is true, the button is added as a toggle switch
	void	addButton(char* fileName,bool makeCheckButton, char* tooltipText);

	//Add the standard copy/cut/paste etc. buttons to the toolbar
	//this functionality is built into the Common Controls library
	void	addStandardButtons();

	//Adds a splitter to the toolbar to seperate icons out
	void	addSplitter();

	//Remove a button from the toolbar
	void	removeButton(int index);  //Not Done

	//Enable or disable a toolbar button
	void	setButtonEnabled(int index, int enabled);

	//Retrieve the enabled/disabled state of a toolbar button
	int		getButtonEnabled(int index);

	//Set the pushed / unpushed state of a toolbar button
	void	setButtonState(int index, int state);

	//Retrieve the pushed / unpushed state of a toolbar button
	int		getButtonState(int index);

	//Retrieves the currently active toolbar item.  Call this after the user clicks on the toolbar
	//to retrieve the zero based index of the button they clicked on.
	int		getHotItem();

private:
	//Handle to the rebar gadget which contains the toolbar
	HWND rebar;

};

//A control with up/down buttons on used to select numerical values
class CGadgetUpDown : public CGadget
{
public:
	//Create the up/down control
	bool create(int x, int y, int width, int height, int style, HWND parent);

	//--Commands--

	//Sets the up/down control's buddy window
	void setBuddy(HWND buddy);

	//Sets the max / min range of the up/down control
	void setRange(int min, int max);

	//Sets the position of the up/down control
	void setPosition(int position);

	//--Expressions--

	//Retrieves the HWND of the buddy window
	HWND getBuddy();

	//Retrieves the maximum allowed value for the up/down control
	int getMaximumValue();

	//Retrieves the minimum allowed value for the up/down control
	int getMinimumValue();

	//Retrieves the current position of the up/down control
	int getPosition();
};

//A control used to display other controls inside it
class CGadgetWindow : public CGadget
{
public:
	//Create window
	bool create(int x, int y, int width, int height, char* caption, DWORD styleOptions, DWORD exStyleOptions, int createTopMost, HWND parent);

	//Maximize window to taskbar
	void maximize();

	//Minimize window to taskbar
	void minimize();

	//Restore to normal size
	void restore();

	//Change the window's icon
	void setIcon(char* fileName);

private:
	HICON myIcon;
};

//A control similar to a scrollbar but with discrete possible values
class CGadgetTrackbar : public CGadget
{
public:
	//Create the trackbar
	bool create(int x, int y, int width, int height, int style, HWND parent);

	//--Commands--

	//Sets the range of the trackbar
	void setRange(int min, int max);

	//Sets the distance between ticks in the trackbar
	void setTickFrequency(int frequency);

	//Sets the position of the thumb
	void setPosition(int position);

	//--Expressions--

	//Retrieves the positon of the trackbar
	int getPosition();

	//Retrieves the minimum range value
	int getMinimumValue();

	//Retrieves the maximum range value
	int getMaximumValue();
};

//A gadget which the user can draw on for custom gadgets
class CGadgetCanvas : public CGadget
{
public:
	//Create the canvas (basically a type of STATIC)
	bool create(int x, int y, int width, int height, HWND parent);

	//Initiates the internal memory DC, must be called when the window is resized
	void initDC(int width, int height);

	//--Commands--

	//Set the drawing colours
	void		setDrawingColor(COLORREF line, COLORREF fill, COLORREF text);

	//Over-ride of setColors
	void		setColors(COLORREF background, COLORREF text);

	//Sets the width of lines drawn
	void		setLineWidth(int width);

	//Draw a line
	void		drawLine(int x1, int y1, int x2, int y2);

	//Draw a rectangle
	void		drawRect(int x1, int y1, int x2, int y2);

	//Draws an ellipse
	void		drawEllipse(int x1, int y1, int x2, int y2);

	//Draws text
	void		drawText(int x, int y, char* text);

	//Draws a pixel
	void		drawPoint(int x, int y, COLORREF color);

	//Retrives the colour of a pixel
	COLORREF	getPoint(int x, int y);

	//Creates a DBPro image from a canvas
	void		getImage(int imageId);


	//Pastes a DBPro image into a canvas
	void		pasteImage(int x, int y, int imageId);

	//Sets the font used by the canvas to draw text
	void		setCanvasFont(CGadgetFont* font);

	//Copies part of an existing canvas
	void		copyFrom(CGadgetCanvas* source, int left, int top, int right, int bottom);

	//Gets a handle to the canvas' device context
	HDC			getCanvasDC();

	//Set the alpha value of the canvas
	virtual
	void		setAlpha(int alpha) {myAlpha=alpha;}

	//Draw canvas
	void drawCanvas();
	

private:
	//Retrieve pointer to bitmap bits
	DWORD* getImageDataPtr();

	//Retrieve width of canvas pen
	int getPenWidth();

	//Update the translucency of the canvas
	void updateAlpha(HDC winDC);

	//Drawing pen
	HPEN myPen;

	//Drawing brush
	HBRUSH myBrush;

	//Drawing font
	HFONT myFont;

	//Size of internal memory DC
	int		myMemoryDCWidth;
	int		myMemoryDCHeight;

	//Alpha value of Canvas
	int		myAlpha;

	
};

//Displays items of data with icons & captions
class CGadgetListview : public CGadget
{
public:
	//Create listview
	bool		create(int x, int y, int width, int height, HWND parentWindow);


	//--Commands--

	//Sets the background and foreground colours of the listivew
	void		setColors(COLORREF background, COLORREF text);

	//Set the image list from which the images for items are obtained
	//Each listview can have up to 3 image lists, one for small, medium & large icons
	void		setImageList(CGadgetImageList* imageList, int listIndex);

	//Set the listview display mode
	void		setViewStyle(int viewStyle);

	//Add a new item to the listview
	void		addItem(char* caption, int image);

	//Add subitems to an existing item in the listview
	void		setItemText(char* caption, int index, int subItemIndex);

	//Gets the text of an item
	void		getItemText(int item, int subItem, char* buffer, int bufferSize);


	//Add a new column to the listview (report mode)
	void		addColumn(char* caption, int width);

	//Removes an item from the listview
	void		removeItem(int index);

	//Selects an item in the listview
	void		selectItem(int index);

	//Returns the index of the currently selected item
	int			selectedItem();

	//Retrieves the index of the item at the specified co-ordinates
	int			hitTestItem(int x, int y);

	//Retrieves the subitem index of the item at the specified co-ordinates
	int			hitTestSubItem(int x, int y);

	//Retrieves the X co-ordinate of an item
	int			getItemX(int item, int subItem);

	//Retrieves the Y co-ordinate of an item
	int			getItemY(int item, int subItem);

	//Sets the width of a given column
	void		setColumnWidth(int column, int width);

	//Retrieves the width of a given column
	int			getColumnWidth(int column);
	
	//--Expressions--

	//Returns the number of items in the listview gadget
	int			getItemCount();

protected:
	
	//Virtuals
	virtual void onSize(int width , int height);

private:
	HIMAGELIST	myImageList;

	int			myColumnCount;
};

//A gadget which can display information as a hierarchical series of nodes
class CGadgetTreeview : public CGadget
{
public:
	//Create treeview gadget
	bool		create(int x, int y, int width, int height, HWND parentWindow);

	//Set the treeview's image list
	void		setImageList(CGadgetImageList* imageList, int index);

	//Add a new item to the treeview
	HTREEITEM	addItem(HTREEITEM parentItem, int image, char* caption, HTREEITEM insertAfter=0);

	//Sets the background and foreground colours in the treeview
	void		setColors(COLORREF background, COLORREF text);

	//Sets the text of an item in the treeview
	void		setItemText(HTREEITEM item, char* text);

	//Retrieves the text of an item in the treeview
	void		getItemText(HTREEITEM item, char* textBuffer, int bufferSize);

	//Gets the handle of an item at the specified point
	HTREEITEM	hitTest(int x, int y);

	//Retrieves a handle to the first child item of the specified item
	HTREEITEM	getFirstChild(HTREEITEM item);

	//Retrieves a handle to the next sibling item of the specified item
	HTREEITEM	getNextSibling(HTREEITEM item);

	//Retrieves a handle to the previous sibling item of the specified item
	HTREEITEM	getPreviousSibling(HTREEITEM item);

	//Retrieves a handle to the parent item of the specified item
	HTREEITEM	getParent(HTREEITEM item);

	//Retrieves a handle to a the root item of a treeview
	HTREEITEM   getRoot();

	//Remove an item from the tree
	void		removeItem(HTREEITEM item);

	//Set the currently selected item
	void		selectItem(HTREEITEM item);

	//Retrieve the currently selected item
	HTREEITEM	selectedItem();

	//Command handler
	virtual bool commandHandler(unsigned short commandCode);
};

class CGadgetWebBrowser : public CGadget
{
public:
	//Create a web browser gadget
	bool create(int x, int y, int width, int height, HWND parent);

	//Adjusts the position and dimensions of the web browser's window
	void move(int xPos, int yPos, int width, int height, bool clientSize=false);

	//Navigate to the specified URL
	void navigate(char* URL);

	//Execute the specified HTML code
	void run(char* html);

	//Retrieve the current URL
	void currentURL(char* buffer, DWORD size);

	virtual
		bool wantTabs() {return true;}
};

class CGadgetProgressBar : public CGadget
{
public:
	//Create a progress bar
	bool create(int x, int y, int width, int height, HWND parent);

	//Sets the colour of the progress bar
	void setColors(COLORREF background, COLORREF text);

	//Sets the progress bar's range
	void setRange(int minValue, int maxValue);

	//Sets the progress bar's position
	void setPosition(int position);

	//Retrieves the min value of the range
	int getMin();

	//Retrieves the max value of the range
	int getMax();

	//Retrieves the current position
	int getPosition();
};

class CGadgetRichEdit : public CGadget
{
public:

	//Create a richedit gadget
	bool			create(int x, int y, int width, int height, HWND parent);

	//Sets the start and end of the current text selection
	void			setSel(int start, int len);
	
	//Sets the font and colour used by the current text selection
	void			setSelFont(CGadgetFont* font, COLORREF color);

	//Retrieves the font of the currently selected text
	CGadgetFont*	getSelFont();

	//Retrieves the color of the currently selected text
	COLORREF		getSelColor();

	//Enables or disables word-wrapping on the control
	void			setWordWrapMode(int wordWrap);

	//Sets the background colour of the gadget
	void			setColors(COLORREF background, COLORREF text);

	//Retrieves the start of the current selection
	int  getSelStart();

	//Retrieves the length of the current selection
	int  getSelLen();

	virtual
		bool wantTabs() {return true;}
};

#endif