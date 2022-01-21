#include <random>
#include <ctime>
#include <vector>

#include "HMenuGenerator.h"

namespace HMenuGenerator
{
	std::vector<HMENU> usedValues;
	std::mt19937 rndGen(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<unsigned long long> uid(1000, 1999);
}

HMENU HMenuGenerator::getNewHMenu()
{
	HMENU tmp = (HMENU)(UINT_PTR)(uid(rndGen));
	for (unsigned int i = 0; i < usedValues.size(); i++)
		if (usedValues[i] == tmp)
			return getNewHMenu();
	usedValues.push_back(tmp);
	return tmp;
}

void HMenuGenerator::releaseHMenu(HMENU hMenu)
{
	for (unsigned int i = 0; i < usedValues.size(); i++)
		if (usedValues[i] == hMenu)
		{
			usedValues.erase(usedValues.begin() + i);
			return;
		}
}