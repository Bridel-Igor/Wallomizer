#pragma once

#include <Windows.h>
#include <mutex>

namespace Internet
{
	extern constexpr DWORD bufferSize = 32768;
	extern char buffer[bufferSize];	// HACK: move to heap and allocate on demand
	extern std::mutex bufferAccess;

	bool URLDownloadToBuffer(const char* URL, char* _buffer = buffer, DWORD _bufferSize = bufferSize);
	bool URLDownloadToFile(const char* URL, const wchar_t* path);
	char* parse(char* buffer, const char* key, nullptr_t value);
	char* parse(char* buffer, const char* key, char* value);
	char* parse(char* buffer, const char* key, unsigned int* value);
}