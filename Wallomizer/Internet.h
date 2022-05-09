#pragma once

#include <Windows.h>
#include <mutex>

namespace Internet
{
	extern constexpr DWORD bufferSize = 32768;
	extern char buffer[bufferSize];	// HACK: move to heap and allocate on demand
	extern std::mutex bufferAccess;

	bool URLDownloadToBuffer(const wchar_t* URL, char* pBuffer = buffer, DWORD _bufferSize = bufferSize);
	bool URLDownloadToFile(const wchar_t* URL, const wchar_t* path);
	char* parse(char* pBuffer, const char* key, nullptr_t value);
	char* parse(char* pBuffer, const char* key, wchar_t* value);
	char* parse(char* pBuffer, const char* key, unsigned int* value);
}