#include "Internet.h"

char Internet::buffer[bufferSize];
std::mutex Internet::bufferAccess;

bool Internet::URLDownloadToBuffer(char* URL, char* _buffer, DWORD _bufferSize)
{
	HINTERNET hInternetSession;
	HINTERNET hURL;
	DWORD dwBytesRead;

	hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternetSession)
		return false;

	hURL = InternetOpenUrlA(hInternetSession, URL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
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

bool Internet::URLDownloadToFile(char* URL, wchar_t* path)
{
	HINTERNET hInternetSession;
	HINTERNET hURL;
	DWORD dwBytesRead;
	
	hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternetSession)
		return false;

	hURL = InternetOpenUrlA(hInternetSession, URL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (!hURL)
	{
		InternetCloseHandle(hInternetSession);
		return false;
	}

	const DWORD tmpBufferSize = 1024;
	char buffer[tmpBufferSize];

	DWORD dwTemp;
	HANDLE hFile = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	bool result = false;
	while (InternetReadFile(hURL, (LPSTR)buffer, tmpBufferSize, &dwBytesRead))
	{
		if (dwBytesRead == 0)
		{
			result = true;
			break;
		}
		WriteFile(hFile, buffer, dwBytesRead, &dwTemp, NULL);
	}

	CloseHandle(hFile);
	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternetSession);
	return result;
}

char* Internet::parse(char* buffer, const char* key, nullptr_t value)
{
	if (buffer == nullptr)
		return nullptr;
	char* pBuffer = buffer;
	pBuffer = strstr(pBuffer, key);
	if (pBuffer == nullptr)
		return nullptr;
	pBuffer += strlen(key);
	return pBuffer;
}

char* Internet::parse(char* buffer, const char* key, char* value)
{
	char* pBuffer = parse(buffer, key, nullptr);
	if (pBuffer == nullptr)
		return nullptr;
	pBuffer++;
	int slide = 0, i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '"')
	{
		if (pBuffer[i] == 92)
		{
			slide++;
			i++;
			continue;
		}
		value[i - slide] = pBuffer[i];
		i++;
	}
	value[i - slide] = '\0';
	return pBuffer;
}

char* Internet::parse(char* buffer, const char* key, unsigned int* value)
{
	char* pBuffer = parse(buffer, key, nullptr);
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