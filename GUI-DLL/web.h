/*
 * This include file is meant to be included with any C source you
 * write which uses the cwebpage DLL.
 */

#ifndef __CWEBPAGE_H_INCLUDED
#define __CWEBPAGE_H_INCLUDED

#include <windows.h>

#ifdef __cplusplus
//{
#endif

long WINAPI EmbedBrowserObject(HWND);
#define EMBEDBROWSEROBJECT EmbedBrowserObject
typedef long WINAPI EmbedBrowserObjectPtr(HWND);

void WINAPI UnEmbedBrowserObject(HWND);
#define UNEMBEDBROWSEROBJECT UnEmbedBrowserObject
typedef long WINAPI UnEmbedBrowserObjectPtr(HWND);

long WINAPI DisplayHTMLPage(HWND, LPCTSTR);
#define DISPLAYHTMLPAGE DisplayHTMLPage
typedef long WINAPI DisplayHTMLPagePtr(HWND, LPCTSTR);

long WINAPI GetCurrentURL(HWND, LPCTSTR, DWORD);
#define GETCURRENTURL GetCurrentURL
typedef long WINAPI GetCurrentURLPtr(HWND, LPCTSTR, DWORD);

long WINAPI DisplayHTMLStr(HWND, LPCTSTR);
#define DISPLAYHTMLSTR DisplayHTMLStr
typedef long WINAPI DisplayHTMLStrPtr(HWND, LPCTSTR);

void WINAPI SetSize(HWND,int,int);
#define SETSIZE SetSize
typedef long WINAPI SetSizePtr(HWND,int,int);

#ifdef __cplusplus
//}
#endif

#endif /* __CWEBPAGE_H_INCLUDED */
