#include "pch.h"
#include "Container.h"

using namespace Minty;

void Minty::Container::set(void const* const data, Size const size)
{
	// resize to new size
	resize(size);

	// set at 0
	set_at(data, size, 0);
}
