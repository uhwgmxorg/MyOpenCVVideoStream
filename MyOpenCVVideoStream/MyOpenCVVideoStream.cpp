/**************************************************************************/
/*  MyOpenCVVideoStream                                                   */
/*  we add c:\opencv\build\x64\vc15\bin\ to the Path EnvVar               */
/*           \opencv\build\include\      to the additional include path   */
/*           \opencv\build\x64\vc15\lib\ to the additional lib path       */
/*           opencv_world401d.lib        to the additional libs           */
/*           and                                                          */
/*           add c:\opencv\build\x64\vc15\bin\ to the PATH Env Var        */
/*  OpenCV is from:                                                       */
/*           https://sourceforge.net/projects/opencvlibrary/              */
/*                                                                        */
/*                                                 uhwgmxorg 02.02.2019   */
/**************************************************************************/
#include "pch.h"
#include <conio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>

#include "..\UhwUtilsDll\UhwUtilsDll.h"
#include "AppSettings.h"
#include "Stopwatch.h"

using namespace std;
using namespace cv;
using namespace win32;

mutex  mtx;                          // lock_guard
bool   bVideoIsRunning = false;
bool   bTakePicture = false;
bool   bIsDetecting = false;
bool   bDetectEyes = false;
double scaleFactor = 1.1;            // 1.1 .ini var
int    minNeighbors = 2;             // 2   .ini var

thread* pVideoWorkerThread = NULL;
HWND    consoleWin;

VideoCapture      cap;
string            cascadeName, nestedCascadeName;
CascadeClassifier cascade, nestedCascade;
double            internalScaleFactror = 3.0;   // 3.0 .ini var
unsigned long     ulPngFileImageNo;

// function forward
void DetectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
void StartVideo();

//-------------------------------------------
//  PrintMenu
//-------------------------------------------
void PrintMenu()
{
    bool r;
    bool d;
    bool e;
    double s;
    int n;
    CString strFileVersion = "0.0.0.0";
    CString strProductVersion = "";
    CString strScaleFactor = "";
    CString strMinNeighbors = "";
    GetVersionOfApplication(&strFileVersion, &strProductVersion);

#ifdef DEBUG
    ScreenOutput(1, 0, "Program MyOpenCVVideoStream Debug Version " + strFileVersion, console_text_colors::light_cyan);
#else
    ScreenOutput(1, 0, "Program MyOpenCVVideoStream Release Version " + strFileVersion, console_text_colors::light_cyan);
#endif // DEBUG

    int X = 22, Y = 2;
    ScreenOutput(X, Y + 0, "s Start/Stop Video");
    ScreenOutput(X, Y + 1, "d Start/Stop Detection");
    ScreenOutput(X, Y + 2, "e Detect Eyes On/Off");
    ScreenOutput(X, Y + 3, "p Take a Picture #");  cout << ulPngFileImageNo;
    ScreenOutput(X, Y + 4, "+\\- increase/decrease scaleFactor");
    ScreenOutput(X, Y + 5, "*\\/ increase/decrease minNeighbors");
    ScreenOutput(X, Y + 6, "c For Beep");
    ScreenOutput(X, Y + 7, "x For Exit");
    ScreenOutput(X + 1, Y + 9, "press a key for choice", console_text_colors::light_green);

    {
        lock_guard<mutex> lock(mtx);
        s = scaleFactor;
        n = minNeighbors; 
    }

    strScaleFactor.Format(_T("scaleFactor=%g"),s);
    strMinNeighbors.Format(_T("minNeighbors=%i"), n);

    ScreenOutput(0, 14, strMinNeighbors, console_text_colors::magenta);
    ScreenOutput(16, 14, strScaleFactor, console_text_colors::magenta);

    int X2 = 49, Y2 = 12;
    {
        lock_guard<mutex> lock(mtx);
        r = bVideoIsRunning;
    }

    if (r)
    { 
        ScreenOutput(X2, Y2 + 0, "Video Streaming", console_text_colors::red); 
        ScreenOutput(X2 + 16, Y2 + 0, "ON", console_text_colors::light_red); 
    }
    else
    {
        ScreenOutput(X2, Y2 + 0, "Video Streaming", console_text_colors::red);
        ScreenOutput(X2 + 16, Y2 + 0, "OFF", console_text_colors::light_red);
    }

    {
        lock_guard<mutex> lock(mtx);
        d = bIsDetecting;
    }

    if (d)
    {
        ScreenOutput(X2, Y2 + 1, "Detection", console_text_colors::red);
        ScreenOutput(X2 + 16, Y2 + 1, "ON", console_text_colors::light_red);
    }
    else
    {
        ScreenOutput(X2, Y2 + 1, "Detection", console_text_colors::red);
        ScreenOutput(X2 + 16, Y2 + 1, "OFF", console_text_colors::light_red);
    }

    {
        lock_guard<mutex> lock(mtx);
        e = bDetectEyes;
    }

    if (e)
    {
        ScreenOutput(X2, Y2 + 2, "Detect Eyes", console_text_colors::red);
        ScreenOutput(X2 + 16, Y2 + 2, "ON", console_text_colors::light_red);
    }
    else
    {
        ScreenOutput(X2, Y2 + 2, "Detect Eyes", console_text_colors::red);
        ScreenOutput(X2 + 16, Y2 + 2, "OFF", console_text_colors::light_red);
    }

    ScreenOutput(80, 0, "");
}

//-------------------------------------------
//  MenuDispatcher 
//-------------------------------------------
void MenuDispatcher(char key)
{
    bool r;
    system("cls");
    switch (key)
    {
    case 's':
        {
            lock_guard<mutex> lock(mtx);
            r = bVideoIsRunning = !bVideoIsRunning;
        }

        if (r)
        {
            pVideoWorkerThread = new thread(StartVideo);
            system("cls");
        }
        else
        {
            {
                lock_guard<mutex> lock(mtx);
                bVideoIsRunning = false;
            }

            Sleep(400);
            system("cls");
        }
        break;
    case 'd':
        {
            lock_guard<mutex> lock(mtx);
            bIsDetecting = !bIsDetecting;
        }
        break;
    case 'e':
        {
            lock_guard<mutex> lock(mtx);
            bDetectEyes = !bDetectEyes;
        }
        break;
    case 'p':
        {
            lock_guard<mutex> lock(mtx);
            r = bVideoIsRunning;
        }

        if (!r)
        {
            cout << '\a';
            cout << '\a';
            ScreenOutput(20, 5, "Start video capture first !!", console_text_colors::red, console_bg_colors::white);
            _getch();
            system("cls");
        }
        else
        {
            ulPngFileImageNo++;

            {
                lock_guard<mutex> lock(mtx);
                bTakePicture = true;
            }
        }
        break;
    case '+':
        {
            lock_guard<mutex> lock(mtx);
            scaleFactor += 0.005;
            if (scaleFactor >= 3) scaleFactor = 3;
        }
    break;
    case '-':
        {
            lock_guard<mutex> lock(mtx);
            scaleFactor -= 0.005;
            if (scaleFactor <= 1.001) scaleFactor = 1.005;
        }
        break;
    case '*':
        {
            lock_guard<mutex> lock(mtx);
            minNeighbors += 1;
            if (minNeighbors >= 6) minNeighbors = 6;
        }
        break;
    case '/':
        {
            lock_guard<mutex> lock(mtx);
            minNeighbors -= 1;
            if (minNeighbors <= 1) minNeighbors = 1;
        }
        break;
    case 'c':
        cout << '\a';
        system("cls");
        break;
    case 'x':
        break;
    default:
        if (key != 0)
        {
            cout << '\a';
            cout << '\a';
            ScreenOutput(18, 5, "You pressed a wrong key !!", console_text_colors::red, console_bg_colors::white);
            _getch();
            system("cls");
        }
        break;
    }
}

//-------------------------------------------
//  StartVideo     
//-------------------------------------------
void StartVideo()
{
    bool r;
    bool p;
    String* strImageFileName;
    char* szImageFileName = new char[MAX_PATH];

    char buffer[200];
    Stopwatch sw;
    double duration = 0.0;
    unsigned int frameCounter = 0;

    {
        lock_guard<mutex> lock(mtx);
        r = bVideoIsRunning;
    }

    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(0))
        return;

    while (r)
    {
        sw.Start();

        {
            lock_guard<mutex> lock(mtx);
            r = bVideoIsRunning;
        }

        Mat frame;
        // calculate and print FPS
        duration = duration / 1000.0;
        sprintf(buffer,"FPS %4.1f Frames %i",1.0/duration, ++frameCounter);
        ScreenOutput(0, 13, buffer, console_text_colors::yellow); ScreenOutput(0, 13, "");

        cap >> frame;
        if (frame.empty()) break; // end of video stream
        Mat frame1 = frame.clone();

        {
            lock_guard<mutex> lock(mtx);
            p = bTakePicture;
        }

        // take a picture
        if (p)
        {
            sprintf(szImageFileName, "image%d.png", ulPngFileImageNo);
            strImageFileName = new String(szImageFileName);
            imwrite(*strImageFileName, frame);
            delete(strImageFileName);

            {
                lock_guard<mutex> lock(mtx);
                p = bTakePicture = false;;
            }

            cout << '\a';
        }

        DetectAndDraw(frame, cascade, nestedCascade, internalScaleFactror);
        waitKey(10);
        sw.Stop();
        duration = sw.ElapsedMilliseconds();
    }
    // the camera will be closed automatically upon exit
    cap.release();
    destroyAllWindows();
    return;
}

//-------------------------------------------
//  DetectAndDraw     
//-------------------------------------------
void DetectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale)
{
    bool e;
    bool d;
    double s;
    int n;
    vector<Rect> faces, faces2;
    Mat gray, smallImg;
    String strWindowsHeadLine;

    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale 
    double fx = 1 / scale;

    // Resize the Grayscale Image  
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);

    strWindowsHeadLine = "Face Detection";

    {
        lock_guard<mutex> lock(mtx);
        d = bIsDetecting;
    }

    if (d)
    {	
        {
            lock_guard<mutex> lock(mtx);
            s = scaleFactor;
            n = minNeighbors;
        }

        try
        {
            // Detect faces of different sizes using cascade classifier  
            cascade.detectMultiScale(smallImg, faces, s, n, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
        }
        catch (const std::exception& ex)
        {
            std::cout << "exception: " << ex.what() << std::endl;
        }

        // Draw circles around the faces 
        for (size_t i = 0; i < faces.size(); i++)
        {
            Rect r = faces[i];
            Mat smallImgROI;
            vector<Rect> nestedObjects;
            Point center;
            Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
            int radius;

            double aspect_ratio = (double)r.width / r.height;
            if (0.75 < aspect_ratio && aspect_ratio < 1.3)
            {
                center.x = cvRound((r.x + r.width*0.5)*scale);
                center.y = cvRound((r.y + r.height*0.5)*scale);
                radius = cvRound((r.width + r.height)*0.25*scale);
                circle(img, center, radius, color, 3, 8, 0);
            }
            else
                rectangle(img, cvPoint(cvRound(r.x*scale),
                    cvRound(r.y*scale)),
                    cvPoint(cvRound((r.x + r.width - 1)*scale),
                        cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
            if (nestedCascade.empty())
                continue;
            smallImgROI = smallImg(r);

            {
                lock_guard<mutex> lock(mtx);
                e = bDetectEyes;
            }

            if (e)
            {
                // Detection of eyes int the input image 
                nestedCascade.detectMultiScale(smallImgROI,
                    nestedObjects,
                    s,
                    n,
                    0 | CASCADE_SCALE_IMAGE,
                    Size(30, 30));

                // Draw circles around eyes 
                for (size_t j = 0; j < nestedObjects.size(); j++)
                {
                    Rect nr = nestedObjects[j];
                    center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
                    center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
                    radius = cvRound((nr.width + nr.height)*0.25*scale);
                    circle(img, center, radius, color, 3, 8, 0);
                }
            }
        }
    }

    // Show Processed Image with detected faces 
    imshow(strWindowsHeadLine, img);
}

//-------------------------------------------
//  GetMyexePath
//-------------------------------------------
string GetMyExePath()
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    string mywPath = &szFilePath[0];

    return mywPath;
}

//-------------------------------------------
//  main
//-------------------------------------------
int main()
{   
    bool r;
    CAppSettings appSettings;

    #pragma region Load Ini Vars
    // load the ini vars and set the window pos
    consoleWin = GetConsoleWindow();
    RECT rec;
    GetWindowRect(consoleWin, &rec);
    rec.left = appSettings.m_Left;
    rec.top = appSettings.m_Top;
    rec.right = appSettings.m_Right;
    rec.bottom = appSettings.m_Bottom;
    ulPngFileImageNo = appSettings.m_PngFileImageNo;
    MoveWindow(consoleWin, rec.left, rec.top, rec.right - rec.left, rec.bottom - rec.top, TRUE);
    scaleFactor = appSettings.m_ScaleFactor;
    minNeighbors = appSettings.m_MinNeighbors;
    internalScaleFactror = appSettings.m_InternalScaleFactror;
    #pragma endregion

    // Get my exe path
#ifdef DEBUG
    cout << "We are here --> " << GetMyExePath() << endl;
#endif // DEBUG

    string path;
    string fileName;
    string pathFile;

    #pragma region Load Xml Files
    // Load classifiers from "opencv/data/haarcascades" directory for the development
    path = "\\opencv\\sources\\data\\haarcascades\\";
    fileName = "haarcascade_eye.xml";
    pathFile = path + fileName;
    // in the release version from the path given in the .ini
    pathFile = appSettings.m_OpenCVXmlPath + appSettings.m_NestedCascadeXmlFile;
    if (!nestedCascade.load(pathFile))
        cout << "ERROR loding " + pathFile << endl;

    // Change path before execution for the development
    path = "\\opencv\\sources\\data\\haarcascades\\";
    fileName = "haarcascade_frontalface_default.xml";
    pathFile = path + fileName;
    // in the release version from the path given in the .ini
    pathFile = appSettings.m_OpenCVXmlPath + appSettings.m_CascadeXmlFile;
    if (!cascade.load(pathFile))
        cout << "ERROR loding " + pathFile << endl;
    #pragma endregion


    CHAR c = 0;
    system("cls");
    do
    {
        MenuDispatcher(c);
        PrintMenu();
        c = _getch();
    } while (c != 'x');


    #pragma region Terminate Video Thread
    // terminate the video thread if still running
    {
        lock_guard<mutex> lock(mtx);
        r = bVideoIsRunning;
    }

    if (r)
    {
        {
            lock_guard<mutex> lock(mtx);
            bVideoIsRunning = false;
        }
    }

    if (pVideoWorkerThread)
        pVideoWorkerThread->join();
    #pragma endregion

    #pragma region Save Ini Vars
    // save the ini vars
    GetWindowRect(consoleWin, &rec);
    appSettings.m_Left = rec.left;
    appSettings.m_Top = rec.top;
    appSettings.m_Right = rec.right;
    appSettings.m_Bottom = rec.bottom;
    appSettings.m_PngFileImageNo = ulPngFileImageNo;

    appSettings.m_ScaleFactor = scaleFactor;
    appSettings.m_MinNeighbors = minNeighbors;
    appSettings.m_InternalScaleFactror = internalScaleFactror;
    #pragma endregion

    return 0;
}