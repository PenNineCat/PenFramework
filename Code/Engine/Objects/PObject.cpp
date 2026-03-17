#include "PObject.h"

void PenFramework::PenEngine::PObject::SetProperty(StringView propertyName, const std::any& property)
{
	if (auto it = m_properties.find(propertyName); it == m_properties.end())
		m_properties.emplace(String(propertyName), property);
	else
		it->second = property;
}
