#pragma once
#include <string>
#include <format>

#define LOGFORMAT(s, o) Log::Write(std::format(s, o))
#define LOGFORMATLINE(s, o) Log::WriteLine(std::format(s, o))

namespace tytas::Log {
	void Write(const char* msg);
	void WriteLine(const char* msg);

	void Write(const std::wstring& msg);
	void WriteLine(const std::wstring& msg);

	void Error(const char* err);
	void Error(const std::wstring& err);
	void Error(const char* err, HRESULT hr);
}