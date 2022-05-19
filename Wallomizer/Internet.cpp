#include "Internet.h"

#pragma comment(lib, "Wininet.lib")

Internet::Internet()
{
	m_hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
}

Internet::~Internet()
{
	InternetCloseHandle(m_hInternetSession);
	if (m_pBuffer)
		delete[] m_pBuffer;
	m_pBuffer = nullptr;
}

bool Internet::DownloadToBuffer(const wchar_t* wsURL, size_t bufferSize)
{
	HINTERNET hURL = InternetOpenUrlW(m_hInternetSession, wsURL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);

	if (m_pBuffer == nullptr)
		m_pBuffer = new char[bufferSize];
	memset(m_pBuffer, 0, bufferSize);
	m_pCurrent = m_pBuffer;

	DWORD dwBytesRead = 0, tmpBufferSize = 1024;
	while (InternetReadFile(hURL, m_pCurrent, tmpBufferSize, &dwBytesRead))
	{
		if (dwBytesRead == 0)
			break;
		m_pCurrent += dwBytesRead;
		bufferSize -= dwBytesRead;
		if (bufferSize < 0)
			break;
	}

	m_pCurrent = m_pBuffer;
	InternetCloseHandle(hURL);
	return true;
}

bool Internet::DownloadToFile(const wchar_t* wsURL, const wchar_t* wsPath)
{
	HINTERNET hURL = InternetOpenUrlW(m_hInternetSession, wsURL, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);

	const DWORD tmpBufferSize = 1024;
	char tmpBuffer[tmpBufferSize]{};

	DWORD dwBytesRead;
	DWORD dwTemp;
	HANDLE hFile = CreateFileW(wsPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
	return result;
}

bool Internet::parse(const char* sKey, bool bFromLastPos)
{
	m_pCurrent = bFromLastPos ? m_pCurrent : m_pBuffer;
	if (m_pCurrent == nullptr)
		return false;

	const size_t len = strlen(sKey) + 4;
	char *sFullKey = new char[len];
	strcpy_s(sFullKey, len, "\"");
	strcat_s(sFullKey, len, sKey);
	strcat_s(sFullKey, len, "\":");

	m_pCurrent = strstr(m_pCurrent, sFullKey);
	delete [] sFullKey;

	if (m_pCurrent == nullptr)
		return false;
	m_pCurrent += len - 1;
	return true;
}

bool Internet::parse(const char* sKey, wchar_t* wsValue, bool bFromLastPos)
{
	if (!parse(sKey, bFromLastPos))
		return false;
	int slide = 0, i = 0;
	m_pCurrent++;
	while (m_pCurrent != nullptr && m_pCurrent[i] != '"')
	{
		if (m_pCurrent[i] == '\\') // special symbols
		{
			// removing '\' symbol. This also converts path separators from '\/' to '/'.
			slide++;
			i++;
			if (m_pCurrent[i] == 'u') // unicode symbol decoding
			{
				const char code[5]{ m_pCurrent[i + 1], m_pCurrent[i + 2], m_pCurrent[i + 3], m_pCurrent[i + 4], '\0' };
				wsValue[i - slide] = (wchar_t)strtol(code, NULL, 16);
				slide += 4;
				i += 5;
			}
			continue;
		}
		wsValue[i - slide] = m_pCurrent[i];
		i++;
	}
	wsValue[i - slide] = '\0';
	m_pCurrent += i;
	return true;
}

bool Internet::parse(const char* sKey, unsigned int& uValue, bool bFromLastPos)
{
	if (!parse(sKey, bFromLastPos))
		return false;
	char search[64] = "";
	int i = 0;
	while (m_pCurrent != nullptr && m_pCurrent[i] != '}' && m_pCurrent[i] != ',')
	{
		search[i] = m_pCurrent[i];
		i++;
	}
	search[i] = '\0';
	m_pCurrent += i;
	uValue = atoi(search);
	return true;
}