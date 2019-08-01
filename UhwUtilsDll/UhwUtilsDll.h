#pragma once

#include <atlstr.h>
#include "conmanip.h"
using namespace conmanip;

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

DECLDIR void GetVersionOfApplication(CString *strFileVersion, CString *strProductVersion);
DECLDIR void ScreenOutput(int x, int y, const char *text);
DECLDIR void ScreenOutput(int x, int y, const char *text, console_text_colors foregroundColor, console_bg_colors backgroundColor);
DECLDIR void ScreenOutput(int x, int y, const char *text, console_text_colors foregroundColor);
DECLDIR void ScreenOutput(int x, int y, CString text);
DECLDIR void ScreenOutput(int x, int y, CString text, console_text_colors foregroundColor, console_bg_colors backgroundColor);
DECLDIR void ScreenOutput(int x, int y, CString text, console_text_colors foregroundColor);
DECLDIR void ScreenOutput(int x, int y, CString *text);
DECLDIR void ScreenOutput(int x, int y, CString *text, console_text_colors foregroundColor, console_bg_colors backgroundColor);
DECLDIR void ScreenOutput(int x, int y, CString *text, console_text_colors foregroundColor);
DECLDIR void ResetColore(); 
