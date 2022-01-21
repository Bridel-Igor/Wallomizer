#include "IClickable.h"
#include "HMenuGenerator.h"

IClickable::IClickable()
{
	m_hMenu = HMenuGenerator::getNewHMenu();
}

IClickable::~IClickable()
{
	HMenuGenerator::releaseHMenu(m_hMenu);
}

bool IClickable::isClicked(WPARAM wParam)
{
	return this && ((HMENU)LOWORD(wParam) == m_hMenu) && (HIWORD(wParam) == BN_CLICKED);
}