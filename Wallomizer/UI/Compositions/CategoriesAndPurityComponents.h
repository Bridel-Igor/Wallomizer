#pragma once

#include <Windows.h>

#include "PushButton.h"
#include "CategoriesAndPurity.h"

class PurityComponent
{
public:
	PurityComponent(IComponent* pParent, int x, int y, int width, int height);
	~PurityComponent() = default;
	void setPurity(CategoriesAndPurity cap) noexcept;
	[[nodiscard]] CategoriesAndPurity getPurity() const noexcept;
	bool draw(LPDRAWITEMSTRUCT pDIS) const noexcept;
	bool click(WPARAM wParam) noexcept;
	void enable(bool enable) noexcept;
	void moveComponent(int x, int y, int width, int height) noexcept;

private:
	PushButton pbSFW, pbSketchy, pbNSFW;
};

class CategoryComponent
{
public:
	CategoryComponent(IComponent* pParent, int x, int y, int width, int height);
	~CategoryComponent() = default;
	void setCategory(CategoriesAndPurity cap) noexcept;
	[[nodiscard]] CategoriesAndPurity getCategory() const noexcept;
	bool draw(LPDRAWITEMSTRUCT pDIS) const noexcept;
	bool click(WPARAM wParam) noexcept;

private:
	PushButton pbGeneral, pbAnime, pbPeople;
};
