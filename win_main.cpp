#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <commctrl.h>
#include <stdio.h>
#include <shlwapi.h>
#include "win_main.h"
#include "d3d11_init.cpp"
#include "game_timer.cpp"

HINSTANCE g_hinst;                          /* This application's HINSTANCE */
HWND g_hwndChild;                           /* Optional child window */

#define _DEBUG   1

global_variable d3d11_basic     GlobalD3D11Basic;
global_variable game_timer      GlobalGameTimer;
global_variable bool            GlobalPause = false;

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
   	//D3D11DrawScene(D3d11Basic);
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
        case WM_ACTIVATE:
        {
            if(LOWORD(wParam) == WA_INACTIVE)
            {
                GlobalPause = true;
                GameTimerStop(&GlobalGameTimer);
            }
            else
            {
                GlobalPause = false;
                GameTimerStart(&GlobalGameTimer);
            }
        }break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
            uint32 vkCode = (uint32)wParam;
            if(vkCode == 'P')
            {
                GlobalPause = true;
            }
			
		}break;
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(hwnd, &Paint);
			OnPaint(hwnd, &GlobalD3D11Basic);
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

internal void
CalculateFrameState(game_timer* GameTimer)
{
    static int32 FrameCount = 0;
    static real32 TimeElapsed = 0.0f;

    FrameCount++;

    if((GameTimerTotalTime(GameTimer) - TimeElapsed) >= 1.0f)
    {
        real32 fps = (real32)FrameCount;
        real32 msPerFrame = 1000.0f / fps;

        // char output_buffer[256];

		// sprintf_s(output_buffer, sizeof(output_buffer),
		// 			"Milliseconds/frame: %.2f ms, FPS: %f\n" ,
		// 		msPerFrame, fps);
        // OutputDebugStringA(output_buffer);

        FrameCount = 0;
        TimeElapsed += 1.0f;
    }
}

internal void
ProcessPendingMessage(d3d11_basic* D3d11Basic, game_timer *GameTimer)
{
    MSG msg = {};
    ResetTimer(GameTimer);

    while(msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            GameTimerTick(GameTimer);

            if(!GlobalPause)
            {
                CalculateFrameState(GameTimer);
                D3D11UpdateScene(GameTimer->DeltaTime);
                D3D11DrawScene(D3d11Basic);
            }
            else
            {
                Sleep(100);
            }
        }
    }
}



int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
                   LPSTR lpCmdLine, int nShowCmd)
{

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

	GlobalD3D11Basic = {};
	InitDirect3D(hwnd,&GlobalD3D11Basic,960,540);

    GlobalGameTimer = {};
    int64 CountsPerSecond;
    QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSecond);
    GlobalGameTimer.SecondsPerCount = 1.0 / (real64)CountsPerSecond;
    

    ProcessPendingMessage(&GlobalD3D11Basic,&GlobalGameTimer);
   
    
    return 0;
}