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
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITORINFOEXA mon;
    mon.cbSize = sizeof(mon);

    ::GetMonitorInfoA(hMonitor, &mon);
    int nxRes = GetDeviceCaps(hdcMonitor, HORZRES);
    int nYRes = GetDeviceCaps(hdcMonitor, VERTRES); 
    std::wstring Arr = IntToWStr(nxRes) + s2ws("x") + IntToWStr(nYRes)+s2ws("\0");
    TextOut(hdcMonitor, 0, 0, Arr.c_str(), 8);
    return TRUE;
}
int main()
{
    while (1) {
        HDC common_dc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        ::EnumDisplayMonitors(common_dc, nullptr, MonitorEnumProc, 0);
    }
    return 0;
}
