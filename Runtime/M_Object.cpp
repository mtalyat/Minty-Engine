#include "pch.h"
#include "M_Object.h"

#include <typeinfo>

using namespace minty;

std::string const Object::toString() const
{
	// return the name of this object
	return typeid(*this).name();
}
