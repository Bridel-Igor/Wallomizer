#include "Internet.h"

#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

char Internet::buffer[bufferSize];
std::mutex Internet::bufferAccess;
// TODO: no internet exception handler
bool Internet::URLDownloadToBuffer(const wchar_t* URL, char* _buffer, DWORD _bufferSize)
{
	HINTERNET hInternetSession;
	HINTERNET hURL;
	DWORD dwBytesRead;

	memset(buffer, 0, bufferSize);

	hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternetSession)
		return false;
	
	hURL = InternetOpenUrlW(hInternetSession, URL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (!hURL)
	{
		InternetCloseHandle(hInternetSession);
		return false;
	}

	char* pBuffer = _buffer;
	DWORD tmpBufferSize = 1024;
	bool result = false;
	while (InternetReadFile(hURL, (LPSTR)pBuffer, tmpBufferSize, &dwBytesRead))
	{
		if (dwBytesRead == 0)
		{
			result = true;
			break;
		}
		pBuffer += dwBytesRead;
		_bufferSize -= dwBytesRead;
		if (_bufferSize < 0)
			break;
	}

	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternetSession);
	return result;
}

bool Internet::URLDownloadToFile(const wchar_t* URL, const wchar_t* path)
{
	HINTERNET hInternetSession;
	HINTERNET hURL;
	DWORD dwBytesRead;
	
	hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternetSession)
		return false;

	hURL = InternetOpenUrlW(hInternetSession, URL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (!hURL)
	{
		InternetCloseHandle(hInternetSession);
		return false;
	}

	const DWORD tmpBufferSize = 1024;
	char tmpBuffer[tmpBufferSize];

	DWORD dwTemp;
	HANDLE hFile = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	bool result = false;
	while (InternetReadFile(hURL, (LPSTR)tmpBuffer, tmpBufferSize, &dwBytesRead))
	{
		if (dwBytesRead == 0)
		{
			result = true;
			break;
		}
		WriteFile(hFile, tmpBuffer, dwBytesRead, &dwTemp, NULL);
	}

	CloseHandle(hFile);
	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternetSession);
	return result;
}

char* Internet::parse(char* _pBuffer, const char* sKey, nullptr_t)
{
	if (_pBuffer == nullptr)
		return nullptr;
	char* pBuffer = _pBuffer;
	pBuffer = strstr(pBuffer, sKey);
	if (pBuffer == nullptr)
		return nullptr;
	pBuffer += strlen(sKey);
	return pBuffer;
}

char* Internet::parse(char* _pBuffer, const char* sKey, wchar_t* wsValue)
{
	char* pBuffer = parse(_pBuffer, sKey, nullptr);
	if (pBuffer == nullptr)
		return nullptr;
	pBuffer++;
	int slide = 0, i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '"')
	{
		if (pBuffer[i] == '\\') // special symbols
		{
			// removing '\' symbol. This also converts path separators from '\/' to '/'.
			slide++;
			i++;
			if (pBuffer[i] == 'u') // unicode symbol decoding
			{
				const char code[5]{ pBuffer[i + 1], pBuffer[i + 2], pBuffer[i + 3], pBuffer[i + 4], '\0' };
				wsValue[i - slide] = (wchar_t)strtol(code, NULL, 16);
				slide += 4;
				i += 5;
			}
			continue;
		}
		wsValue[i - slide] = pBuffer[i];
		i++;
	}
	wsValue[i - slide] = '\0';
	return pBuffer;
}

char* Internet::parse(char* _pBuffer, const char* key, unsigned int* value)
{
	char* pBuffer = parse(_pBuffer, key, nullptr);
	if (pBuffer == nullptr)
		return nullptr;
	char search[64] = "";
	int i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '}' && pBuffer[i] != ',')
	{
		search[i] = pBuffer[i];
		i++;
	}
	search[i] = '\0';
	*value = atoi(search);
	return pBuffer;
}