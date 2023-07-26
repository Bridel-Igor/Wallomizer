#pragma once

#include <Windows.h>
#include <stdio.h>

#include "Wallpaper.h"
#include "CategoriesAndPurity.h"

class BaseCollection
{
public:
	BaseCollection() = default;
	BaseCollection(const BaseCollection&) = delete;
	BaseCollection(BaseCollection&&) = delete;
	BaseCollection& operator=(const BaseCollection&) = delete;
	BaseCollection& operator=(BaseCollection&&) = delete;

	virtual ~BaseCollection() {}
	virtual bool saveSettings(FILE* pFile) const = 0;
	virtual bool loadSettings(FILE* pFile, unsigned short fileVersion) = 0;
	virtual void getCollectionName(wchar_t* pwsName, size_t size) const = 0;
	virtual CollectionType getCollectionType() const = 0;
	virtual CategoriesAndPurity getCAP() const = 0;
	virtual Wallpaper* getWallpaperInfo(unsigned int index) const = 0;
	virtual void openCollectionSettingsWindow(HWND hWnd) = 0;

	unsigned int getNumber() const	{ return m_uiNumber; }
	bool isEnabled() const			{ return m_isEnabled; }
	void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
	bool isValid() const			{ return m_isValid; }
	void setValid(bool isValid)		{ m_isValid = isValid; }

protected:
	bool m_isEnabled = true;
	bool m_isValid = false;
	unsigned int m_uiNumber = 0;
};