#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include "ProcessHandler.h"

HANDLE ProcessHandler::handle = nullptr;
bool ProcessHandler::is_connected = false;

void ProcessHandler::connect() {
	disconnect();
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(snapshot, &entry)) {
		do {
			if (_tcsicmp(entry.szExeFile, L"TY.exe") == 0) {
				handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				is_connected = true;
				break;
			}
		} while (Process32Next(snapshot, &entry));
	}
	CloseHandle(snapshot);
}

void ProcessHandler::disconnect() {
	if (is_connected) {
		CloseHandle(handle);
		is_connected = false;
	}
}

bool ProcessHandler::try_get_handle(HANDLE* h) {
	*h = handle;
	return is_connected;
}