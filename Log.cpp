#include <windows.h>
#include <debugapi.h>
#include <sstream>
#include <iostream>
#include <string>
#include <comdef.h>
#include "Log.h"
#include <cstdarg>

const bool WRITETOFILE = false;
std::wostringstream ss;

void _print() {
	OutputDebugString(ss.str().c_str());
	std::wostringstream().swap(ss);
}

void tytas::Log::Write(const char* msg) {
	ss << msg;
	_print();
}

void tytas::Log::WriteLine(const char* msg) {
	ss << msg << std::endl;
	_print();
}

void tytas::Log::Write(const std::wstring& msg) {
	ss << msg;
	_print();
}

void tytas::Log::WriteLine(const std::wstring& msg) {
	ss << msg << std::endl;
	_print();
}

void tytas::Log::Error(const char* err) {
	ss << "ERROR: " << err << std::endl;
	_print();
}

void tytas::Log::Error(const std::wstring& err) {
	ss << "ERROR: " << err << std::endl;
	_print();
}

void tytas::Log::Error(const char* msg, HRESULT hr) {
	_com_error err(hr);
	ss << msg << std::endl;
	ss << err.ErrorMessage() << std::endl;
	_print();
}

