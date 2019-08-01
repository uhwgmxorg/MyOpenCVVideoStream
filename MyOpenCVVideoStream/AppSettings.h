#pragma once

#include <string>

#define SZ_VALUE_SIZE 300

class CAppSettings
{
public:
    long    m_Left = 10;
    long    m_Top = 10;
    long    m_Right = 580;
    long    m_Bottom = 280;

    unsigned long m_PngFileImageNo = 0;

    double m_ScaleFactor = 1.1;     // 1.1
    int    m_MinNeighbors = 2;      // 2
    double m_InternalScaleFactror;   // 1

    std::string m_OpenCVXmlPath = ".\\";
    std::string m_NestedCascadeXmlFile = "haarcascade_eye.xml";
    std::string m_CascadeXmlFile = "haarcascade_frontalface_default.xml";

    std::string m_IniFlileName = ".\\MyOpenCVVideoStream.ini";

public:
    CAppSettings();
    CAppSettings(std::string intFile);
    ~CAppSettings();
    unsigned int LoadConfig();
    unsigned int SaveConfig();

    static bool FileExsist(char *lpszFilename);
};

