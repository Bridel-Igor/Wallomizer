#include "CategoriesAndPurityComponents.h"

PurityComponent::PurityComponent(HWND hParent, int x, int y, int width, int height) :
	pbSFW		(hParent, "SFW",		x,							y, width / 3 - 1,	height, 0, 0, RGB(85,150,85), RGB(55,88,55)),
	pbSketchy	(hParent, "Sketchy",	x + width / 3,				y, width / 3,		height, 0, 0, RGB(150,150,85), RGB(88,88,55)),
	pbNSFW		(hParent, "NSFW",		1 + x + (2 * width / 3),	y, width / 3 - 1,	height, 0, 0, RGB(150,85,85), RGB(88,55,55))
{
}

void PurityComponent::setPurity(CategoriesAndPurity cap)
{
	pbSFW.check(cap & CAP::puritySFW);
	pbSketchy.check(cap & CAP::puritySketchy);
	pbNSFW.check(cap & CAP::purityNSFW);
}

CategoriesAndPurity PurityComponent::getPurity() const
{
	return	CAP::puritySFW * pbSFW.isChecked() |
			CAP::puritySketchy * pbSketchy.isChecked() |
			CAP::purityNSFW * pbNSFW.isChecked();
}

bool PurityComponent::draw(LPDRAWITEMSTRUCT &pDIS)
{
	if (pDIS->hwndItem == pbSFW.hWnd())
	{
		pbSFW.draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbSketchy.hWnd())
	{
		pbSketchy.draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbNSFW.hWnd())
	{
		pbNSFW.draw(pDIS);
		return true;
	}
	return false;
}

bool PurityComponent::click(WPARAM wParam)
{
	if (pbSFW.isClicked(wParam))
	{
		pbSFW.check(!pbSFW.isChecked());
		return true;
	}
	if (pbSketchy.isClicked(wParam))
	{
		pbSketchy.check(!pbSketchy.isChecked());
		return true;
	}
	if (pbNSFW.isClicked(wParam))
	{
		pbNSFW.check(!pbNSFW.isChecked());
		return true;
	}
	return false;
}

void PurityComponent::moveComponent(int x, int y, int width, int height)
{
	MoveWindow(pbSFW.hWnd(),		x,							y, width / 3 - 1,	height, FALSE);
	MoveWindow(pbSketchy.hWnd(),	x + width / 3,				y, width / 3,		height, FALSE);
	MoveWindow(pbNSFW.hWnd(),		1 + x + (2 * width / 3),	y, width / 3 - 1,	height, FALSE);
}

void PurityComponent::mouseHovering(WPARAM wParam)
{
	pbSFW.mouseHovering(wParam);
	pbSketchy.mouseHovering(wParam);
	pbNSFW.mouseHovering(wParam);
}

// CategoryComponent section

CategoryComponent::CategoryComponent(HWND hParent, int x, int y, int width, int height) :
	pbGeneral	(hParent, "General",	x,						y,	width / 3 - 1,	height),
	pbAnime		(hParent, "Anime",		x + width / 3,			y,	width / 3,		height),
	pbPeople	(hParent, "People",		1+x + (2 * width / 3),	y,	width / 3-1,	height)
{
}

void CategoryComponent::setCategory(CategoriesAndPurity cap)
{
	pbGeneral.check(cap & CAP::categoryGeneral);
	pbAnime.check(cap & CAP::categoryAnime);
	pbPeople.check(cap & CAP::categoryPeople);
}

CategoriesAndPurity CategoryComponent::getCategory() const
{
	return	CAP::categoryGeneral * pbGeneral.isChecked() |
			CAP::categoryAnime * pbAnime.isChecked() |
			CAP::categoryPeople * pbPeople.isChecked();
}

bool CategoryComponent::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (pDIS->hwndItem == pbGeneral.hWnd())
	{
		pbGeneral.draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbAnime.hWnd())
	{
		pbAnime.draw(pDIS);
		return true;
	}
	if (pDIS->hwndItem == pbPeople.hWnd())
	{
		pbPeople.draw(pDIS);
		return true;
	}
	return false;
}

bool CategoryComponent::click(WPARAM wParam)
{
	if (pbGeneral.isClicked(wParam))
	{
		pbGeneral.check(!pbGeneral.isChecked());
		return true;
	}
	if (pbAnime.isClicked(wParam))
	{
		pbAnime.check(!pbAnime.isChecked());
		return true;
	}
	if (pbPeople.isClicked(wParam))
	{
		pbPeople.check(!pbPeople.isChecked());
		return true;
	}
	return false;
}

void CategoryComponent::mouseHovering(WPARAM wParam)
{
	pbGeneral.mouseHovering(wParam);
	pbAnime.mouseHovering(wParam);
	pbPeople.mouseHovering(wParam);
}