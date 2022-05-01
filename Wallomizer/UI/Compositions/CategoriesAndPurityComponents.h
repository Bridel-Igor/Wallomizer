#pragma once

#include <Windows.h>

#include "PushButton.h"
#include "CategoriesAndPurity.h"

class PurityComponent
{
public:
	PurityComponent(HWND hParent, int x, int y, int width, int height);
	~PurityComponent() {}
	void setPurity(CategoriesAndPurity cap);
	CategoriesAndPurity getPurity() const;	
	bool draw(LPDRAWITEMSTRUCT &pDIS);
	bool click(WPARAM wParam);
	void moveComponent(int x, int y, int width, int height);
	void mouseHovering(WPARAM wParam);

private:
	PushButton pbSFW, pbSketchy, pbNSFW;
};

class CategoryComponent
{
public:
	CategoryComponent(HWND hParent, int x, int y, int width, int height);
	~CategoryComponent() {}
	void setCategory(CategoriesAndPurity cap);
	CategoriesAndPurity getCategory() const;
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	bool click(WPARAM wParam);
	void mouseHovering(WPARAM wParam);

private:
	PushButton pbGeneral, pbAnime, pbPeople;
};