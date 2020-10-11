#include "FileInterface.h"
#include "LibraryCaller.h"


#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <shobjidl.h> 

// Global variables

#define ID_OPENBUTTON1 0x8801
#define ID_RUNBUTTON 0x8802
// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Filtrowanie obrazu");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }



    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    HWND hwndSelectButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Select",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        100,         // x position 
        300,         // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_OPENBUTTON1,       
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    HWND hwndRunButton = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Uruchom",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        300,         // x position 
        300,         // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)ID_RUNBUTTON,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
    //SendMessage(hwndButton, BCM_SETNOTE, 0, (LPARAM)L"with note");
    HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), LibraryCaller::instance().outputName.c_str(),
        WS_CHILD | WS_VISIBLE, 100, 20, 140,
        20, hWnd, NULL, NULL, NULL);


    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Plik wejściowy:");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        

        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        TextOut(hdc,
            5, 20,
            LibraryCaller::instance().inputName.c_str(), LibraryCaller::instance().inputName.length());
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        if ((HIWORD(wParam) == BN_CLICKED) && (lParam != 0))
        {
            switch (LOWORD(wParam))
            {
            case ID_OPENBUTTON1: {
                HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                    COINIT_DISABLE_OLE1DDE);
                if (SUCCEEDED(hr))
                {
                    IFileOpenDialog* pFileOpen;

                    // Create the FileOpenDialog object.
                    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                    if (SUCCEEDED(hr))
                    {
                        // Show the Open dialog box.
                        hr = pFileOpen->Show(NULL);

                        // Get the file name from the dialog box.
                        if (SUCCEEDED(hr))
                        {
                            IShellItem* pItem;
                            hr = pFileOpen->GetResult(&pItem);
                            if (SUCCEEDED(hr))
                            {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Display the file name to the user.
                                if (SUCCEEDED(hr))
                                {
                                    LibraryCaller::instance().inputName = pszFilePath;
                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                        pFileOpen->Release();
                    }
                    CoUninitialize();
                }
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE); }
                break;

            case ID_RUNBUTTON:
                LibraryCaller::instance().Run();
            break;
            }
        }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

//int main(int argc, char* arg[])
//{
//    LibraryCaller library;
//    library.ParseArgs(argc, arg);
//
//    uint8_t* pixels = nullptr;
//    BITMAPFILEHEADER* bmpHeader = nullptr;
//    BITMAPINFOHEADER* bmpInfo = nullptr;
//    ReadBMP("test.bmp", pixels, bmpHeader, bmpInfo);
//    uint8_t* newPixels = new uint8_t[bmpInfo->biSizeImage];
//    int i = library.ProcessImage(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
//    SaveBMP("testsave.bmp", newPixels, bmpHeader, bmpInfo);
//
//    delete[] pixels;
//    delete[] newPixels;
//    delete bmpHeader;
//    delete bmpInfo;
//}
