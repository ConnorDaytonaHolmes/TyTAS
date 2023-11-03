// TyTas.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include "framework.h"
#include "TyTAS.h"
#include "Log.h"
#include "File.h"
#include "ProcessHandler.h"
#include "Run.h"
#include "Input.h"

#define MAX_LOADSTRING 100

using namespace tytas;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TYTAS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    LOGFORMAT(L"Run length (in frames): ", Run::get_frame_length());

    Run::add_input_to_frame(180, JUMP_DOWN);
    Run::add_input_to_frame(200, JUMP_UP);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TYTAS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYTAS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TYTAS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   int windowWidth = 1440;
   int windowHeight = 810;
   HWND hWnd = CreateWindowW(
       szWindowClass,           // Class name { "Ty TAS" }
       szTitle,                 // Window title { "Ty TAS" }
       WS_OVERLAPPEDWINDOW,     // dwStyle
       CW_USEDEFAULT,           // x
       0,                       // y
       windowWidth,             // width
       windowHeight,            // height
       nullptr,                 // hWndParent
       nullptr,                 // hMenu
       hInstance,               // instance handle
       nullptr                  // lpParam
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define MF_ENABLE_IF(b) b ? MF_ENABLED : MF_DISABLED

void UpdateMenuItems(HMENU hMenu) {

    int menuItemCount = GetMenuItemCount(hMenu);
    Log::Write("Menu item count: ");
    Log::WriteLine(std::to_wstring(menuItemCount));

    HMENU tySubMenu = GetSubMenu(hMenu, 1);
    
    int tySubMenuItemCount = GetMenuItemCount(tySubMenu);
    Log::Write("TY SubMenu item count: ");
    Log::WriteLine(std::to_wstring(tySubMenuItemCount));


    /*MENUITEMINFO tsmi;
    memset(&tsmi, 0, sizeof(tsmi));
    tsmi.cbSize = sizeof(tsmi);
    tsmi.fMask = MIIM_STRING | MIIM_ID | MIIM_FTYPE | MIIM_STATE;

    for (int i = 0; i < tySubMenuItemCount; i++) {
        WCHAR smi_name[20];
        int gotname = GetMenuString(tySubMenu, i, smi_name, 20, MF_BYPOSITION);
        Log::WriteLine(std::format(L"Ty_MenuItem[{}] : {}", i, smi_name));

        if (!GetMenuItemInfo(tySubMenu, i, true, &tsmi)) {
            Log::Error(std::format(L"Failed to get menu item info for submenu '{}'", smi_name));
            continue;
        }
        LOGFORMATLINE(L"wID: {}", tsmi.wID);
        LOGFORMATLINE(L"fState: {}", tsmi.fState);

        if ((tsmi.fType & MF_STRING) != 0 && tsmi.cch > 0) {
            tsmi.cch++;
            TCHAR* pString = (TCHAR*)malloc(tsmi.cch * sizeof(TCHAR));
            if (pString != NULL) {
                Log::WriteLine(std::format(L"TySubMenuItem {}:, name: {}", i, pString));
            }
            free(pString);
        }
    }*/


    //Disable all TY.exe menu items if disconnected
    EnableMenuItem(hMenu, ID_TY_START, MF_ENABLE_IF(!ProcessHandler::is_connected)); //TODO: grey out if no TY.exe path
    EnableMenuItem(hMenu, ID_TY_RESTART, MF_ENABLE_IF(ProcessHandler::is_connected));
    EnableMenuItem(hMenu, ID_TY_EXIT, MF_ENABLE_IF(ProcessHandler::is_connected));


    // Enable connect if not connected, otherwise disable it if we are connected
    // vice versa for disconnect
    EnableMenuItem(hMenu, ID_PROCESS_CONNECT, MF_ENABLE_IF(!ProcessHandler::is_connected));
    EnableMenuItem(hMenu, ID_PROCESS_DISCONNECT, MF_ENABLE_IF(ProcessHandler::is_connected));

    //Disable all Run menu items if disconnected, TODO: otherwise enable/disable based on run state
    EnableMenuItem(hMenu, ID_RUN_PLAY, MF_ENABLE_IF(ProcessHandler::is_connected && Run::is_stopped()));
    EnableMenuItem(hMenu, ID_RUN_STOP, MF_ENABLE_IF(ProcessHandler::is_connected && !Run::is_stopped()));
    
    
    //Set the Pause/Resume menu item name & state depending on run / process state
    HMENU runSM = GetSubMenu(hMenu, 2);
    MENUITEMINFO pauseInfo {};
    pauseInfo.cbSize = sizeof(pauseInfo);
    pauseInfo.fMask = MIIM_STRING | MIIM_STATE;
    pauseInfo.fState = MF_ENABLE_IF(ProcessHandler::is_connected && !Run::is_stopped());
    pauseInfo.dwTypeData = Run::pause_menu_item_label;
    SetMenuItemInfo(runSM, 1, true, &pauseInfo);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_FILE_NEW:
                break;
            case ID_FILE_OPEN:
                file::Open();
                break;
            case ID_FILE_SAVE:
                break;
            case ID_FILE_SAVEAS:
                break;

            case ID_TY_START:
                //TODO: connect to process if one is found, else start new process
                break;
            case ID_TY_RESTART:
                //TODO: get path, close process, reopen
                break;
            case ID_TY_EXIT:
                Run::stop();
                //TODO: close process
                break;

            case ID_PROCESS_CONNECT:
                ProcessHandler::connect();
                Log::Write("ProcessConnected: ");
                Log::WriteLine(std::to_wstring(ProcessHandler::is_connected));
                break;
            case ID_PROCESS_DISCONNECT:
                Run::stop();
                ProcessHandler::disconnect();
                break;

            case ID_RUN_PLAY:
                Run::play();
                break;
            case ID_RUN_PAUSE:
                Run::pause();
                break;
            case ID_RUN_STOP:
                Run::stop();
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        UpdateMenuItems(GetMenu(hWnd));
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        ProcessHandler::disconnect();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}




// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
