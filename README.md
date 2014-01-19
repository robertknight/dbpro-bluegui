BlueGUI v2
==========

This is the source code for the BlueGUI v2, an add-on for DarkBASIC Professional
which provides facilities to create and use native GUI controls in DBPro
apps.

The final public release of BlueGUI v2 was v2.054.
A copy of this can be found in the Installers/ directory.

## Registration Details

The plugin has a very crude registration key mechanism.
To enable plugin functionality, the 'startBlue' function
must be called with a UserID and Key. The following can be used:

startBlue RT, 4e2d

## Source Files

BlueGUI requires Visual Studio 2008 or later and the Windows SDK to build.

GUI-DLL/
	C++ source for the BlueGUI v2 DBPro plugin

GUI2 Build/
	Source code for BlueGUI examples, the Rainbow visual UI editor,
	Help files (HTML format) and pre-built binaries
