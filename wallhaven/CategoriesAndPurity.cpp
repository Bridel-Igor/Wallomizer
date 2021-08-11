#include "CategoriesAndPurity.h"

PurityComponent::PurityComponent(HWND hParent, int x, int y, int width, int height)
{
	hmSFW = HMenuGenerator::getNewHMenu();
	hmSketchy = HMenuGenerator::getNewHMenu();
	hmNSFW = HMenuGenerator::getNewHMenu();

	hwSFW = CreateWindowExA(NULL, TEXT("Button"), "SFW", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x, y, width / 3, height, hParent, hmSFW, NULL, NULL);
	hwSketchy = CreateWindowExA(NULL, TEXT("Button"), "Sketchy", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x + width / 3, y, width / 3, height, hParent, hmSketchy, NULL, NULL);
	hwNSFW = CreateWindowExA(NULL, TEXT("Button"), "NSFW", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x + (2 * width / 3), y, width / 3, height, hParent, hmNSFW, NULL, NULL);
}

PurityComponent::~PurityComponent()
{
	DestroyWindow(hwSFW);
	DestroyWindow(hwSketchy);
	DestroyWindow(hwNSFW);
	HMenuGenerator::releaseHMenu(hmSFW);
	HMenuGenerator::releaseHMenu(hmSketchy);
	HMenuGenerator::releaseHMenu(hmNSFW);
}

void PurityComponent::setPurity(CategoriesAndPurity cap)
{
	SendMessageA(hwSFW, BM_SETCHECK, (WPARAM)(cap & S_PURITY_SFW), NULL);
	SendMessageA(hwSketchy, BM_SETCHECK, (WPARAM)(cap & S_PURITY_SKETCHY), NULL);
	SendMessageA(hwNSFW, BM_SETCHECK, (WPARAM)(cap & S_PURITY_NSFW), NULL);
}

CategoriesAndPurity PurityComponent::getPurity()
{
	return	S_PURITY_SFW * SendMessageA(hwSFW, BM_GETCHECK, NULL, NULL) |
		S_PURITY_SKETCHY * SendMessageA(hwSketchy, BM_GETCHECK, NULL, NULL) |
		S_PURITY_NSFW * SendMessageA(hwNSFW, BM_GETCHECK, NULL, NULL);
}

CategoryComponent::CategoryComponent(HWND hParent, int x, int y, int width, int height)
{
	hmGeneral = HMenuGenerator::getNewHMenu();
	hmAnime = HMenuGenerator::getNewHMenu();
	hmPeople = HMenuGenerator::getNewHMenu();

	hwGeneral = CreateWindowExA(NULL, TEXT("Button"), "General", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x, y, width / 3, height, hParent, hmGeneral, NULL, NULL);
	hwAnime = CreateWindowExA(NULL, TEXT("Button"), "Anime", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x + width / 3, y, width / 3, height, hParent, hmAnime, NULL, NULL);
	hwPeople = CreateWindowExA(NULL, TEXT("Button"), "People", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x + (2 * width / 3), y, width / 3, height, hParent, hmPeople, NULL, NULL);
}

CategoryComponent::~CategoryComponent()
{
	DestroyWindow(hwGeneral);
	DestroyWindow(hwAnime);
	DestroyWindow(hwPeople);
	HMenuGenerator::releaseHMenu(hmGeneral);
	HMenuGenerator::releaseHMenu(hmAnime);
	HMenuGenerator::releaseHMenu(hmPeople);
}

void CategoryComponent::setCategory(CategoriesAndPurity cap)
{
	SendMessageA(hwGeneral, BM_SETCHECK, (WPARAM)(cap & S_CATEGORY_GENERAL), NULL);
	SendMessageA(hwAnime, BM_SETCHECK, (WPARAM)(cap & S_CATEGORY_ANIME), NULL);
	SendMessageA(hwPeople, BM_SETCHECK, (WPARAM)(cap & S_CATEGORY_PEOPLE), NULL);
}

CategoriesAndPurity CategoryComponent::getCategory()
{
	return	S_CATEGORY_GENERAL * SendMessageA(hwGeneral, BM_GETCHECK, NULL, NULL) |
		S_CATEGORY_ANIME * SendMessageA(hwAnime, BM_GETCHECK, NULL, NULL) |
		S_CATEGORY_PEOPLE * SendMessageA(hwPeople, BM_GETCHECK, NULL, NULL);
}