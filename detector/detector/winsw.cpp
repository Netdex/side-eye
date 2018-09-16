#include <cstdio>
#include <windows.h>
#include <cmath>
#include "winsw.h"
#include <string>

BOOL CALLBACK MonitorGetEnumProc(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	MONITORINFOEX mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);

	GetMonitorInfo(hMonitor, &mi);
	std::string s(mi.szDevice);
	reinterpret_cast<std::vector<int>*>(dwData)->push_back(stoi(s.substr(11)));

	return TRUE;
}

BOOL CALLBACK MonitorSetProc(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	MONITORINFOEX mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);

	GetMonitorInfo(hMonitor, &mi);
	if (dwData) {
		if (strcmp(mi.szDevice, (char*)dwData) == 0)
		{
			HWND topWindowHwnd = GetTopWindow(NULL);
			while (topWindowHwnd)
			{
				if (IsWindowVisible(topWindowHwnd)) {
					HMONITOR mon = MonitorFromWindow(topWindowHwnd, 0);
					MONITORINFOEX mix;
					mix.cbSize = sizeof(MONITORINFOEX);
					GetMonitorInfo(mon, &mix);
					if (strcmp(mix.szDevice, mi.szDevice) == 0)
					{
						char buf[256];
						GetWindowText(topWindowHwnd, buf, 256);
						if (strlen(buf) > 0) {
							RECT rt;
							GetWindowRect(topWindowHwnd, &rt);
							printf("%d %d\n", rt.left, rt.top);
							printf("%s\n", buf);
							INPUT input = { 0 };
							input.type = INPUT_MOUSE;
							input.mi.mouseData = 0;
							input.mi.dx = (rt.left + 6) * 65536 / GetSystemMetrics(SM_CXSCREEN);
							input.mi.dy = (rt.top + 1) * 65536 / GetSystemMetrics(SM_CYSCREEN);
							input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
							SendInput(1, &input, sizeof(input));

							input.mi.dx = 0;
							input.mi.dy = 0;
							input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
							SendInput(1, &input, sizeof(input));
							return TRUE;
						}
					}
				}
				topWindowHwnd = GetNextWindow(topWindowHwnd, GW_HWNDNEXT);
			}
		}
	}
	return TRUE;
}

std::vector<int> query_display_ids()
{
	std::vector<int> ids;
	EnumDisplayMonitors(NULL, NULL, MonitorGetEnumProc, (LPARAM)&ids);
	return ids;
}

bool set_active_display(int displayId)
{
	std::string path = "\\\\.\\DISPLAY" + std::to_string(displayId);
	return EnumDisplayMonitors(NULL, NULL, MonitorSetProc, (LPARAM)path.c_str());
}
