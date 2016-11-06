// 1412280_TrashHijack.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412280_TrashHijack.h"
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <codecvt>
using namespace std;

#define MAX_LOADSTRING 100

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
    LoadStringW(hInstance, IDC_MY1412280_TRASHHIJACK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412280_TRASHHIJACK));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412280_TRASHHIJACK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(15+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1412280_TRASHHIJACK);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      350, 150, 500, 350, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
inline int GetFilePointer(HANDLE FileHandle) {
	return SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
}
void SaveTextFile(LPCWSTR text) {
	wfstream output("Text_Clipboard.txt", ios::out|ios::app);
	output.imbue(locale(output.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	wstring tmp(text);
	tmp += L"\n";
	output.write(tmp.c_str(), tmp.size());
	output.close();
}
bool SaveBMPFile(const WCHAR* filename, HBITMAP bitmap, HDC bitmapDC, int width, int height) {
	bool Success = 0;
	HDC SurfDC = NULL;
	HBITMAP OffscrBmp = NULL;
	HDC OffscrDC = NULL;
	LPBITMAPINFO lpbi = NULL;
	LPVOID lpvBits = NULL;
	HANDLE BmpFile = INVALID_HANDLE_VALUE;
	BITMAPFILEHEADER bmfh;
	if ((OffscrBmp = CreateCompatibleBitmap(bitmapDC, width, height)) == NULL)
		return 0;
	if ((OffscrDC = CreateCompatibleDC(bitmapDC)) == NULL)
		return 0;
	HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);
	BitBlt(OffscrDC, 0, 0, width, height, bitmapDC, 0, 0, SRCCOPY);
	if ((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
		return 0;
	ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	SelectObject(OffscrDC, OldBmp);
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
		return 0;
	if ((lpvBits = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
		return 0;
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, lpvBits, lpbi, DIB_RGB_COLORS))
		return 0;
	if ((BmpFile = CreateFile(filename,
		GENERIC_WRITE,
		0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL)) == INVALID_HANDLE_VALUE)
		return 0;
	DWORD Written;
	bmfh.bfType = 19778;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return 0;
	if (Written < sizeof(bmfh))
		return 0;
	if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
		return 0;
	if (Written < sizeof(BITMAPINFOHEADER))
		return 0;
	int PalEntries;
	if (lpbi->bmiHeader.biCompression == BI_BITFIELDS)
		PalEntries = 3;
	else PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
		(int)(1 << lpbi->bmiHeader.biBitCount) : 0;
	if (lpbi->bmiHeader.biClrUsed)
		PalEntries = lpbi->bmiHeader.biClrUsed;
	if (PalEntries) {
		if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
			return 0;
		if (Written < PalEntries * sizeof(RGBQUAD))
			return 0;
	}
	bmfh.bfOffBits = GetFilePointer(BmpFile);
	if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
		return 0;
	if (Written < lpbi->bmiHeader.biSizeImage)
		return 0;
	bmfh.bfSize = GetFilePointer(BmpFile);
	SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return 0;
	if (Written < sizeof(bmfh))
		return 0;
	CloseHandle(BmpFile);
	return 1;
}
string Get2CharOfNum(int n) {
	return (n < 10) ? ("0" + to_string(n)) : to_string(n);
}
bool flagRun = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, IDC_TIMER, 2000, TIMERPROC(NULL));
    switch (message)
    {
	case WM_CREATE:
	{
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		HFONT hFont = CreateFont(30, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
		HWND hWndStatic=CreateWindowEx(0, L"static", L"APP TRASH HIJACK", WS_CHILD | WS_VISIBLE, 110, 20, 400, 100, hWnd, 0, 0, 0);
		SendMessage(hWndStatic, WM_SETFONT, WPARAM(hFont), TRUE);

		hWndStatic = CreateWindowEx(0, L"static", L"Made by: Tran Hoang Lam \n           ID:1412280", WS_CHILD | WS_VISIBLE, 80, 90, 400, 100, hWnd, 0, 0, 0);
		SendMessage(hWndStatic, WM_SETFONT, WPARAM(hFont), TRUE);

		hWndStatic = CreateWindowEx(0, L"static", L"Activate with Ctrl + Alt + Shift + C", WS_CHILD | WS_VISIBLE, 50, 220, 400, 100, hWnd, 0, 0, 0);
		SendMessage(hWndStatic, WM_SETFONT, WPARAM(hFont), TRUE);
		RegisterHotKey(hWnd, IDC_HOTKEY_ACTIVITY, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 0x43);
		break;
	}
	case WM_TIMER:
		if (wParam == IDC_TIMER && flagRun) {
			if (OpenClipboard(hWnd))
			{
				//get text
				if (IsClipboardFormatAvailable(CF_UNICODETEXT))
				{
					HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
					LPCWSTR buffer = (LPCWSTR)GlobalLock(hClipboardData);
					SaveTextFile(buffer);
					GlobalUnlock(hClipboardData);
				}

				//get image
				else {
					HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

					time_t t = time(0);   // get time now
					struct tm * now = localtime(&t);
					string dateTime;

					dateTime = "Image_Screen_Clipboard//Screen " + to_string(now->tm_year%100) + Get2CharOfNum(now->tm_mon) + Get2CharOfNum(now->tm_mday) + " " + Get2CharOfNum(now->tm_hour) + Get2CharOfNum(now->tm_min) + Get2CharOfNum(now->tm_sec) + ".bmp";

					wstring convertDateTime(dateTime.begin(), dateTime.end());
					BITMAP bitmap;
					GetObject(hBitmap, sizeof(BITMAP), &bitmap);

					HDC     hScreen = GetDC(NULL);
					HDC     hDC = CreateCompatibleDC(hScreen);
					HGDIOBJ old_obj = SelectObject(hDC, hBitmap);

					SaveBMPFile(convertDateTime.c_str(), hBitmap, hDC, bitmap.bmWidth, bitmap.bmHeight);
					DeleteDC(hDC);
					ReleaseDC(NULL, hScreen);
					DeleteObject(hBitmap);
				}
				CloseClipboard();
			}
		}
		break;
	case WM_HOTKEY:
	{
		if (LOWORD(wParam) == IDC_HOTKEY_ACTIVITY) {
			if (flagRun == false) {
				ShowWindow(hWnd, SW_HIDE);
				flagRun = true;
			}
			else {
				ShowWindow(hWnd, SW_SHOW);
				flagRun = false;
			}
		}
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
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
