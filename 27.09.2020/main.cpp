#include <Windows.h>
#include <SetupAPI.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <winuser.h>
#include <sstream>
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
std::wstring IntToWStr(int i) {
    std::stringstream ss;
    ss << i;
    std::string str = ss.str();
    return s2ws(str);
}
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <Shellapi.h>
#include "mmsystem.h"
#define TIMER_1 12000
#define SIZE 100
int size = 0;
MONITORINFOEX A[SIZE];
BOOL MonitorEnumProc(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data) {
    MONITORINFOEX info;
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &info);
    A[size] = info;
    size++;
    return TRUE;
}
RECT glRectField1 = { 0,0,300,300 };
#define AppWindowName L"Application"
HINSTANCE hIns;
HWND    HwndMainWindow;
ATOM RegisterClassMainWindow();
HWND CreateMainWindow();
LRESULT CALLBACK WndProcMain(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    MSG msg;
    hIns = hInstance;
    HwndMainWindow = CreateMainWindow();
    SetTimer(HwndMainWindow, TIMER_1, 100, NULL);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
ATOM RegisterClassMainWindow() {

    WNDCLASSEX WndClass;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_DBLCLKS;
    WndClass.lpfnWndProc = WndProcMain;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hIns;
    WndClass.hIcon = NULL;
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = AppWindowName;
    WndClass.hIconSm = NULL;
    return RegisterClassEx(&WndClass);
}
HWND CreateMainWindow() {

    HWND hwnd;
    RegisterClassMainWindow();
    hwnd = CreateWindowEx(
        WS_EX_CONTROLPARENT | WS_EX_APPWINDOW,    // расширенный стиль окна
        AppWindowName,            // имя зарегестрированного класса окна
        L"27.09.2020.exe",           // заголовок окна
        WS_OVERLAPPEDWINDOW,      // стиль окна
        10,                       // X-координата левого верхнего угла
        10,                       // Y-координата левого верхнего угла
        300,                      // ширина окна
        300,                      // высота окна
        NULL,                     // описатель родительского окна
       NULL,// описатель главного меню (для главного окна)
        hIns,
        NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, L"Ошибка создания основного окна приложения", L"", MB_OK);
        return NULL;
    }
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return hwnd;
}
LRESULT CALLBACK WndProcMain(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    HDC	hdc;
    RECT rect;              // структура задающая прямоугольник
    PAINTSTRUCT ps;
    DWORD i;
    int d;
    HDC common_dc;
    switch (iMsg) {
    case WM_CREATE:
        
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        common_dc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        EnumDisplayMonitors(common_dc, NULL, (MONITORENUMPROC)MonitorEnumProc, NULL);
        for (int i = 0; i < size; i++) {
            std::wstring Arr = IntToWStr(A[i].rcMonitor.right) + s2ws("x") + IntToWStr(A[i].rcMonitor.bottom) + s2ws("\0");
            HFONT hFont = ::CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
            SelectObject(hdc, hFont);
            TextOut(hdc, 50 * i, 0, Arr.c_str(), Arr.size());
            DeleteObject(hFont);
        }
        EndPaint(hwnd, &ps);
        return 0;
    case WM_TIMER:
        size = 0;
        InvalidateRect(hwnd, &glRectField1, TRUE);
        return 0;
    case WM_CLOSE:
        break;
    case WM_QUIT:
        break;
        return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
