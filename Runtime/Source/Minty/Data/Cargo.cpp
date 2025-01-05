#include "pch.h"
#include "Cargo.h"

using namespace Minty;

void Minty::Cargo::collect(Container& container) const
{
	for (auto const& [name, slot] : m_slots)
	{
		container.append(slot.container.data(), slot.container.size());
	}
}
