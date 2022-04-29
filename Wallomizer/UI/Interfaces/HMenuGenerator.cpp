#include <random>
#include <ctime>
#include <set>

#include "HMenuGenerator.h"

namespace HMenuGenerator
{
	std::set<HMENU> usedValues;
	std::mt19937 rndGen(static_cast<unsigned int>(time(0)));
	const std::uniform_int_distribution<unsigned long long> uid(1000, 1999);
}

HMENU HMenuGenerator::getNewHMenu()
{
	HMENU tmp = nullptr;
	do { tmp = (HMENU)(UINT_PTR)(uid(rndGen)); } 
	while (usedValues.find(tmp) != usedValues.end());
	usedValues.insert(tmp);
	return tmp;
}

void HMenuGenerator::releaseHMenu(HMENU hMenu)
{
	usedValues.erase(hMenu);
}