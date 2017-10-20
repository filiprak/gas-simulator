// gas-simulator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gas-simulator.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


GasContainerType app_type;
float mm_per_pixelX10;


void showUsage() {
	MessageBox(NULL, L"Usage: command <l -left / r - right>", L"Error", MB_OK);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// cmd args
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
		return 0;
	}
	if (argCount < 2)
	{
		showUsage();
		return 0;
	}
	if (lstrcmp(szArgList[1], L"r") == 0)
		app_type = Right;
	else if (lstrcmp(szArgList[1], L"l") == 0)
		app_type = Left;
	else
	{
		showUsage();
		return 0;
	}

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GASSIMULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GASSIMULATOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GASSIMULATOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GASSIMULATOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   if (app_type == Left) {
	   SetWindowText(hWnd, L"gas-simulator (left)");
	   SetWindowPos(hWnd, HWND_TOP, 50, 50, 500, 500, SWP_NOOWNERZORDER);
   }
   if (app_type == Right) {
	   SetWindowText(hWnd, L"gas-simulator (right)");
	   SetWindowPos(hWnd, HWND_TOP, 550, 50, 500, 500, SWP_NOOWNERZORDER);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetTimer(hWnd, 1, REFRESH_INTERVAL, NULL);

   // register messages
   registerMessages();

   // check screen resolution
   HDC screen = GetDC(NULL);
   int hSize = GetDeviceCaps(screen, HORZSIZE);
   int hRes = GetDeviceCaps(screen, HORZRES);
   mm_per_pixelX10 = 10 * (float)hSize / hRes;   // millimeter per pixel

   // init particle gas container
   RECT rect;
   GetClientRect(hWnd, &rect);
   initGasContainer(app_type,
	   INITIAL_PARTICLES_NUM,
	   PARTICLE_RADIUS,
	   (float)rect.right * mm_per_pixelX10, (float)rect.bottom * mm_per_pixelX10);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HBITMAP bitmap;
	HDC hdc;
	HDC hdcMem;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// handle particle enter messages
	if (message == MSG_FROM_LEFT && app_type == Right) {
		handleMsgFromLeft(wParam, lParam);
		return 0;
	}
	if (message == MSG_FROM_RIGHT && app_type == Left) {
		handleMsgFromRight(wParam, lParam);
		return 0;
	}

	// handle rest of messages
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_TIMER:
	{
		updateGasContainerSize((float)rect.right * mm_per_pixelX10, (float)rect.bottom * mm_per_pixelX10);
		// run gas simulation
		runGasSimulation();
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	case WM_PAINT:
		// double buffering
		hdc = BeginPaint(hWnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);

		// create and clear bitmap
		bitmap = CreateCompatibleBitmap(hdcMem, rect.right, rect.bottom);
		SelectObject(hdcMem, bitmap);
		FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// set map mode for gas particles
		SetMapMode(hdcMem, MM_LOMETRIC);
		SetViewportOrgEx(hdcMem, 0, rect.bottom, NULL);
		
		// paint gas particles
		SelectObject(hdcMem, GetStockObject(BLACK_BRUSH));
		drawGasContainer(hdcMem);

		// adjust bitmap
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0,
			rect.right * mm_per_pixelX10, rect.bottom * mm_per_pixelX10, SRCCOPY);
		
		DeleteObject(bitmap);
		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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

void DebugLog(const char* format, ...) {
	va_list args;
	va_start(args, format);
	char line[128];
	vsprintf(line, format, args);
	OutputDebugStringA(line);
	va_end(args);
}