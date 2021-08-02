#pragma once

#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

namespace Internet
{
	bool URLDownloadToBuffer(char* URL, char* buffer, DWORD bufferSize);
	bool URLDownloadToFile(char* URL, char* path);
	char* parse(char* buffer, const char* key, nullptr_t value);
	char* parse(char* buffer, const char* key, char* value);
	char* parse(char* buffer, const char* key, unsigned int* value);
}