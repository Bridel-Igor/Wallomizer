#include "HMenuGenerator.h"
#include "time.h"

HMENU HMenuGenerator::getNewHMenu()
{
	if (usedValues.empty())
		srand(time(NULL));
	HMENU tmp = (HMENU)(UINT_PTR)(rand() % 1000 + 1000);
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