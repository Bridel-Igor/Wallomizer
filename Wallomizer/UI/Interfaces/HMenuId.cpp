#include "HMenuId.h"

#include <stdexcept>

std::bitset<1000> HMenuId::s_usedValues;

HMenuId::HMenuId()
{
	for (std::size_t i = 0; i<s_usedValues.size(); i++)
	{
		if (!s_usedValues[i])
		{
			s_usedValues[i] = true;
			m_value = static_cast<std::uint16_t>(1000 + i);
			return;
		}
	}
	throw std::runtime_error("HMenuId limit reached");
}

HMenuId::~HMenuId()
{
	if (m_value)
		s_usedValues[m_value - 1000] = false;
}
