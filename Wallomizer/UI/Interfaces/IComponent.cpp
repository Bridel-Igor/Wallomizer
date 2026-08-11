#include "IComponent.h"

#include <algorithm>
#include <stack>

IComponent::IComponent(IComponent* pParent) :
	m_pParent(pParent)
{
	if (m_pParent)
		m_pParent->m_children.push_back(this);
}

IComponent::~IComponent()
{
	if (m_pParent)
	{
		auto it = std::find(m_pParent->m_children.begin(), m_pParent->m_children.end(), this);
		m_pParent->m_children.erase(it);
	}
}

void IComponent::traverseChildren(const std::function<void(IComponent* pComponent)>& operation)
{
	std::stack<IComponent*> depth;
	for (IComponent* child : m_children)
		depth.push(child);
	while (!depth.empty())
	{
		IComponent* component = depth.top();
		depth.pop();

		operation(component);

		for (IComponent* child : component->m_children)
			depth.push(child);
	}
}
