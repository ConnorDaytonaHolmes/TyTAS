#pragma once
#include <winnt.h>

class ProcessHandler {
public:
	static HANDLE handle;
	static bool is_connected;

	static void connect();
	static void disconnect();
	static bool try_get_handle(HANDLE* h);
};