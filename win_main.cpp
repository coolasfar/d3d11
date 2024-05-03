#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <commctrl.h>
#include <shlwapi.h>
#include "win_main.h"
#include "d3d11_init.cpp"


HINSTANCE g_hinst;                          /* This application's HINSTANCE */
HWND g_hwndChild;                           /* Optional child window */

#define _DEBUG   1
d3d11_basic D3D11Basic;


void
OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

/*
 *  OnPaint
 *      Paint the content as part of the paint cycle.
 */
void
OnPaint(HWND hwnd, d3d11_basic *D3d11Basic)
{
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
   	D3D11DrawScene(D3d11Basic);
    EndPaint(hwnd, &ps);
}
/*
 *  OnPrintClient
 *      Paint the content as requested by USER.
 */

void
PaintContent(HWND hwnd, PAINTSTRUCT *ps)
{

}
void
OnPrintClient(HWND hwnd, HDC hdc)
{
    PAINTSTRUCT ps;
    ps.hdc = hdc;
    GetClientRect(hwnd, &ps.rcPaint);
    PaintContent(hwnd, &ps);
}

window_dimension GetWindowDimension(HWND window)
{
	window_dimension result;
	RECT clientRect;
	GetClientRect(window, &clientRect);
	result.Width = clientRect.right - clientRect.left;
	result.Height = clientRect.bottom - clientRect.top;

	return result;
}

/*
 *  Window procedure
 */
LRESULT CALLBACK
WndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uiMsg) 
	{
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);	
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			
		}break;
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			OnPaint(hwnd, &D3D11Basic);
			EndPaint(hwnd, &Paint);
		}break;
		case WM_PRINTCLIENT: 
		{
			OnPrintClient(hwnd, (HDC)wParam); 
			return 0;
		}
    }
    return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}

BOOL
InitApp(WNDCLASS &wc)
{
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hinst;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("D3D11");

    if (!RegisterClass(&wc)) 
		return FALSE;
  //  InitCommonControlsEx();             /* In case we use a common control */
    return TRUE;
}


int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
                   LPSTR lpCmdLine, int nShowCmd)
{
    MSG msg;
    HWND hwnd;
    g_hinst = hinst;
	WNDCLASS wc = {};
    if (!InitApp(wc)) return 0;

    hwnd = CreateWindow(
        TEXT("D3D11"),                /* Class Name */
        TEXT("D3D11"),                /* Title */
        WS_OVERLAPPEDWINDOW,            /* Style */
        CW_USEDEFAULT, CW_USEDEFAULT,   /* Position */
        CW_USEDEFAULT, CW_USEDEFAULT,   /* Size */
        NULL,                           /* Parent */
        NULL,                           /* No menu */
        hinst,                          /* Instance */
        0);                             /* No special parameters */
    ShowWindow(hwnd, nShowCmd);

	D3D11Basic = {};
	InitDirect3D(hwnd,&D3D11Basic,960,540);

    while (GetMessage(&msg, NULL, 0, 0)) 
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
   
    
    return 0;
}