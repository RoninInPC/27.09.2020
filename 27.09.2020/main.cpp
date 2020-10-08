#include <Windows.h>
#include <SetupAPI.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <winuser.h>
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITORINFOEXA mon;
    mon.cbSize = sizeof(mon);

    ::GetMonitorInfoA(hMonitor, &mon);
    std::cout<<"Name:"<< mon.szDevice <<" left="<<mon.rcMonitor.left<<" rigth="<< mon.rcMonitor.right << " top=" << mon.rcMonitor.top << " bottom=" << mon.rcMonitor.bottom<<" nabor flagov="<< mon.dwFlags<<std::endl;
    return TRUE;
}

int main()
{
    HDC common_dc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    ::EnumDisplayMonitors(common_dc, nullptr, MonitorEnumProc, 0);
}
