#pragma once

#include <memory>

/// Provides lazy, shared lifetime management for a resource bundle of type T.
///
/// The first instance creates the bundle. Subsequent instances share it.
/// The bundle is destroyed when the last SharedResources<T> instance is destroyed.
/// The static weak reference does not extend its lifetime.
template <typename T>
class SharedResources
{
public:
	SharedResources()
	{
		m_resource = s_resource.lock();
		if (!m_resource)
		{
			m_resource = std::make_shared<T>();
			s_resource = m_resource;
		}
	}
	const T& get() const noexcept { return *m_resource; }

private:
	std::shared_ptr<T> m_resource;
	static std::weak_ptr<T> s_resource;
};

template <typename T>
std::weak_ptr<T> SharedResources<T>::s_resource;
