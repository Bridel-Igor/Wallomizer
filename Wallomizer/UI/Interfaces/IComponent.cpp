#include <stack>

#include "IComponent.h"

IComponent::IComponent(IComponent* pParent) :
	m_pParent(pParent)
{
	if (m_pParent)
		m_pParent->m_pChildren.push_back(this);
}

IComponent::~IComponent()
{
	if (m_pParent)
		m_pParent->m_pChildren.remove(this);
	m_pChildren.clear();
}

void IComponent::traverseChildren(std::function<void(IComponent* pComponent)> operation)
{
	std::stack<std::pair<std::list<IComponent*>::iterator, std::list<IComponent*>::iterator>> depth;
	depth.push(std::make_pair(this->m_pChildren.begin(), this->m_pChildren.end()));
	bool popped = false;
	do
	{
		while (popped == false && (*depth.top().first)->m_pChildren.size())
			depth.push(std::make_pair((*depth.top().first)->m_pChildren.begin(), (*depth.top().first)->m_pChildren.end()));

		operation(*depth.top().first);

		depth.top().first++;
		popped = false;
		while (depth.top().first == depth.top().second)
		{
			depth.pop();
			popped = true;
			if (depth.empty())
				break;
		}
	} while (depth.size());
}