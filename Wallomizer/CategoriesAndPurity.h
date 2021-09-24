#pragma once

#include <Windows.h>

#include "PushButton.h"

typedef char CategoriesAndPurity;

#define	S_CATEGORY_GENERAL	0b00000001
#define	S_CATEGORY_ANIME	0b00000010
#define	S_CATEGORY_PEOPLE	0b00000100
#define	S_PURITY_SFW		0b00001000
#define	S_PURITY_SKETCHY	0b00010000
#define	S_PURITY_NSFW		0b00100000

class PurityComponent
{
public:
	PurityComponent(HWND hParent, int x, int y, int width, int height);
	~PurityComponent();
	void setPurity(CategoriesAndPurity cap);
	CategoriesAndPurity getPurity();	
	bool draw(LPDRAWITEMSTRUCT &pDIS);
	bool click(WPARAM &wParam);

private:
	PushButton *pbSFW, *pbSketchy, *pbNSFW;
};

class CategoryComponent
{
public:
	CategoryComponent(HWND hParent, int x, int y, int width, int height);
	~CategoryComponent();
	void setCategory(CategoriesAndPurity cap);
	CategoriesAndPurity getCategory();
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	bool click(WPARAM& wParam);

private:
	PushButton* pbGeneral, * pbAnime, * pbPeople;
};