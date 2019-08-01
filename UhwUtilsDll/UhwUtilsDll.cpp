// UhwUtilsDll.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"

#define TRACE ATLTRACE
#include <atlstr.h>

#define DLL_EXPORT
#include "UhwUtilsDll.h"

#include <iostream>
#include <conio.h>

using namespace std;

//-------------------------------------------
//  GetVersionOfApplication for MS VC++
//      we need version.lib
//      strFileVersion the FileVersion
//      strProductVersion the ProductVersion
//-------------------------------------------
DECLDIR void GetVersionOfApplication(CString * strFileVersion, CString * strProductVersion)
{
    *strFileVersion = "strFileVersion";
    *strProductVersion = "strProductVersion";

    // give your application full path
    // get the filename of the executable containing the version resource
    TCHAR szFilename[MAX_PATH + 1] = { 0 };
    if (GetModuleFileName(NULL, szFilename, MAX_PATH) == 0)
    {
        TRACE("GetModuleFileName failed with error %d\n", GetLastError());
        return;
    }
    DWORD dwDummy;
    DWORD dwFVISize = GetFileVersionInfoSize(szFilename, &dwDummy);

    if (dwFVISize == 0)
    {
        *strFileVersion = "we have no FileVersion";
        *strProductVersion = "we have no ProductVersion";
        return;
    }

    LPBYTE lpVersionInfo = new BYTE[dwFVISize];

    GetFileVersionInfo(szFilename, 0, dwFVISize, lpVersionInfo);

    UINT uLen;
    VS_FIXEDFILEINFO *lpFfi;

    VerQueryValue(lpVersionInfo, _T("\\"), (LPVOID *)&lpFfi, &uLen);

    DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
    DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;

    DWORD dwProductVersionMS = lpFfi->dwProductVersionMS;
    DWORD dwProductVersionLS = lpFfi->dwProductVersionLS;

    delete[] lpVersionInfo;

    DWORD dwLeftMostFileVersion = HIWORD(dwFileVersionMS);
    DWORD dwSecondLeftFileVersion = LOWORD(dwFileVersionMS);
    DWORD dwSecondRightFileVersion = HIWORD(dwFileVersionLS);
    DWORD dwRightMostFileVersion = LOWORD(dwFileVersionLS);

    DWORD dwLeftMostProductVersion = HIWORD(dwProductVersionMS);
    DWORD dwSecondLeftProductVersion = LOWORD(dwProductVersionMS);
    DWORD dwSecondRightProductVersion = HIWORD(dwProductVersionLS);
    DWORD dwRightMostProductVersion = LOWORD(dwProductVersionLS);

    strFileVersion->Format(_T("File-Version: %d.%d.%d.%d"), dwLeftMostFileVersion, dwSecondLeftFileVersion, dwSecondRightFileVersion, dwRightMostFileVersion);
    strProductVersion->Format(_T("Product-Version: %d.%d.%d.%d"), dwLeftMostProductVersion, dwSecondLeftProductVersion, dwSecondRightProductVersion, dwRightMostProductVersion);
}

//-------------------------------------------
//  ScreenOutput  overloaded function
//-------------------------------------------
void ScreenOutput(int x, int y, const char* text)
{
#ifdef _UNICODE
    wcout << setposx(x); wcout << setposy(y); wcout << text;
#else
    cout << setposx(x); cout << setposy(y); cout << text;
#endif // UNICODE
}
void ScreenOutput(int x, int y, const char* text, console_text_colors foregroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor);
    cout << setposx(x); cout << setposy(y); cout << text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, const char* text, console_text_colors foregroundColor, console_bg_colors backgroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    cout << setposx(x); cout << setposy(y); cout << text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, string text)
{
#ifdef _UNICODE
    wcout << setposx(x); wcout << setposy(y); wcout << text;
#else
    cout << setposx(x); cout << setposy(y); cout << text;
#endif // UNICODE
}
void ScreenOutput(int x, int y, string text, console_text_colors foregroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor);
    cout << setposx(x); cout << setposy(y); cout << text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, string text, console_text_colors foregroundColor, console_bg_colors backgroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    cout << setposx(x); cout << setposy(y); cout << text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, string* text)
{
#ifdef _UNICODE
    wcout << setposx(x); wcout << setposy(y); wcout << *text;
#else
    cout << setposx(x); cout << setposy(y); cout << *text;
#endif // UNICODE
}
void ScreenOutput(int x, int y, string* text, console_text_colors foregroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << *text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor);
    cout << setposx(x); cout << setposy(y); cout << *text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, string* text, console_text_colors foregroundColor, console_bg_colors backgroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << *text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    cout << setposx(x); cout << setposy(y); cout << *text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString text)
{
#ifdef _UNICODE
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)text;
#else
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)text;
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString text, console_text_colors foregroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor);
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString text, console_text_colors foregroundColor, console_bg_colors backgroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString* text)
{
#ifdef _UNICODE
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)* text;
#else
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)* text;
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString* text, console_text_colors foregroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)* text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor);
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)* text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ScreenOutput(int x, int y, CString* text, console_text_colors foregroundColor, console_bg_colors backgroundColor)
{
#ifdef _UNICODE
    wcout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    wcout << setposx(x); wcout << setposy(y); wcout << (LPCTSTR)* text;
    wcout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#else
    cout << settextcolor(foregroundColor) << setbgcolor(backgroundColor);
    cout << setposx(x); cout << setposy(y); cout << (LPCTSTR)* text;
    cout << settextcolor(console_text_colors::white) << setbgcolor(console_bg_colors::black);
#endif // UNICODE
}
void ResetColore()
{
    const int GRAY = 7;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY);
}