#include <cstdio>
#include <windows.h>
#include <cmath>

BOOL CALLBACK MyInfoEnumProc(
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
							INPUT input = {0};
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
	else
	{
		printf("%s\n", mi.szDevice);
	}

	return TRUE;
}

int main(int argc, char* argv[])
{
	//if (argc < 2) return 1;
	//if (strcmp(argv[1], "dump") == 0)
	//{
	//	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
	//}
	//else if (strcmp(argv[1], "set") == 0)
	//{
		//if (argc < 3) return 1;
		EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, (LPARAM)"\\\\.\\DISPLAY1");//(LPARAM)argv[2]
	//}
	//printf("\n\n\EnumDisplayDevices\n\n\n");

	//DISPLAY_DEVICE dd;
	//ZeroMemory(&dd, sizeof(dd));
	//dd.cb = sizeof(dd);
	//for (int i = 0; EnumDisplayDevices(NULL, i, &dd, 0); i++)
	//{
	//	printf("\n\nDevice %d:", i);
	//	printf("\n    DeviceName:   '%s'", dd.DeviceName);
	//	printf("\n    DeviceString: '%s'", dd.DeviceString);
	//	printf("\n    StateFlags:   %s%s%s%s%s",
	//		((dd.StateFlags &
	//			DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) ?
	//			"desktop " : ""),
	//			((dd.StateFlags &
	//				DISPLAY_DEVICE_PRIMARY_DEVICE) ?
	//				"primary " : ""),
	//				((dd.StateFlags & DISPLAY_DEVICE_VGA_COMPATIBLE) ?
	//					"vga " : ""),
	//					((dd.StateFlags &
	//						DISPLAY_DEVICE_MULTI_DRIVER) ?
	//						"multi " : ""),
	//						((dd.StateFlags &
	//							DISPLAY_DEVICE_MIRRORING_DRIVER) ?
	//							"mirror " : ""));


	//	// Get more info about the device
	//	DISPLAY_DEVICE dd2;
	//	ZeroMemory(&dd2, sizeof(dd2));
	//	dd2.cb = sizeof(dd2);
	//	EnumDisplayDevices(dd.DeviceName, 0, &dd2, 0);
	//	printf("\n    DeviceID: '%s'", dd2.DeviceID);
	//	printf("\n    Monitor Name: '%s'", dd2.DeviceString);
	//}

	//printf("\n\n\nEnumDisplayMonitors\n\n\n");

	//

	//getchar();

	return 0;
}
