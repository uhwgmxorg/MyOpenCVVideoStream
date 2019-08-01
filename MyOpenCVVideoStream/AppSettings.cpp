#include "pch.h"
#include "AppSettings.h"

#include <atlstr.h>
#include <stdio.h>
#include  <io.h>

using namespace std;

//-------------------------------------------
//  Constructor
//-------------------------------------------
CAppSettings::CAppSettings()
{
    LoadConfig();
}

//-------------------------------------------
//  Constructor
//-------------------------------------------
CAppSettings::CAppSettings(string intFile)
{
    m_IniFlileName = intFile;
    LoadConfig();
}

//-------------------------------------------
//  Destructor
//-------------------------------------------
CAppSettings::~CAppSettings()
{
    SaveConfig();
}

//-------------------------------------------
//  LoadConfig
//-------------------------------------------
unsigned int CAppSettings::LoadConfig()
{
    char szValue[SZ_VALUE_SIZE];

    char szIniFile[MAX_PATH];
    strcpy(szIniFile, m_IniFlileName.c_str());

    m_Left = GetPrivateProfileInt("Window", "Left", 10, szIniFile);
    m_Top = GetPrivateProfileInt("Window", "Top", 10, szIniFile);
    m_Right = GetPrivateProfileInt("Window", "Right", 580 + 10, szIniFile);
    m_Bottom = GetPrivateProfileInt("Window", "Bottom", 280 + 10, szIniFile);

    m_PngFileImageNo = GetPrivateProfileInt("Settings", "PngFileImageNo", 0, szIniFile);

    GetPrivateProfileString("OpenCV", "ScaleFactor", "1.1", szValue, SZ_VALUE_SIZE, szIniFile);
    m_ScaleFactor = atof(szValue);
    m_MinNeighbors = GetPrivateProfileInt("OpenCV", "MinNeighbors", 2, szIniFile);
    GetPrivateProfileString("OpenCV", "InternalScaleFactror", "3.0", szValue, SZ_VALUE_SIZE, szIniFile);
    m_InternalScaleFactror = atof(szValue);

    GetPrivateProfileString("OpenCV", "OpenCVXmlPath", m_OpenCVXmlPath.c_str(), szValue, SZ_VALUE_SIZE, szIniFile);
    m_OpenCVXmlPath = szValue;
    GetPrivateProfileString("OpenCV", "HaarcascadeEyeXmlFile", m_NestedCascadeXmlFile.c_str(), szValue, SZ_VALUE_SIZE, szIniFile);
    m_NestedCascadeXmlFile = szValue;
    GetPrivateProfileString("OpenCV", "HaarcascadeFaceXmlFile", m_CascadeXmlFile.c_str(), szValue, SZ_VALUE_SIZE, szIniFile);
    m_CascadeXmlFile = szValue;

    unsigned int e = GetLastError();

    return e;
}

//-------------------------------------------
//  SaveConfig
//-------------------------------------------
unsigned int CAppSettings::SaveConfig()
{
    bool flag;
    char szValue[SZ_VALUE_SIZE];

    char szIniFile[MAX_PATH];
    strcpy(szIniFile, m_IniFlileName.c_str());

    sprintf(szValue, "%d", m_Left);
    flag = WritePrivateProfileString("Window", "Left", szValue, szIniFile);
    sprintf(szValue, "%d", m_Top);
    flag = WritePrivateProfileString("Window", "Top", szValue, szIniFile);
    sprintf(szValue, "%d", m_Right);
    flag = WritePrivateProfileString("Window", "Right", szValue, szIniFile);
    sprintf(szValue, "%d", m_Bottom);
    flag = WritePrivateProfileString("Window", "Bottom", szValue, szIniFile);
    sprintf(szValue, "%d", m_PngFileImageNo);
    flag = WritePrivateProfileString("Settings", "PngFileImageNo", szValue, szIniFile);

    sprintf(szValue, "%f",m_ScaleFactor);
    flag = WritePrivateProfileString("OpenCV", "ScaleFactor", szValue, szIniFile);
    sprintf(szValue, "%d", m_MinNeighbors);
    flag = WritePrivateProfileString("OpenCV", "MinNeighbors", szValue, szIniFile);
    sprintf(szValue, "%f", m_InternalScaleFactror);
    flag = WritePrivateProfileString("OpenCV", "InternalScaleFactror", szValue, szIniFile);

    sprintf(szValue, "%s", m_OpenCVXmlPath.c_str());
    flag = WritePrivateProfileString("OpenCV", "OpenCVXmlPath", szValue, szIniFile);
    sprintf(szValue, "%s", m_NestedCascadeXmlFile.c_str());
    flag = WritePrivateProfileString("OpenCV", "HaarcascadeEyeXmlFile", szValue, szIniFile);
    sprintf(szValue, "%s", m_CascadeXmlFile.c_str());
    flag = WritePrivateProfileString("OpenCV", "HaarcascadeFaceXmlFile", szValue, szIniFile);

    unsigned int e = GetLastError();

    return e;
}

//-------------------------------------------
//  FileExsist 
//-------------------------------------------
bool CAppSettings::FileExsist(char *lpszFilename)
{
    return _access(lpszFilename, 0) == 0;
}
