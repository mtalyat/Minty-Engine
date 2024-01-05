#include "pch.h"
#include "M_Wrapper.h"

#include "M_Console.h"

using namespace minty;

Wrapper::Wrapper()
{
}

void minty::Wrapper::emplace(Wrap const& wrap)
{
	_wraps.push_back(wrap);
}

void minty::Wrapper::emplace(Path const& path)
{
	emplace(Wrap(path));
}

Wrap* minty::Wrapper::find_by_path(String const& name)
{
	for (Wrap& wrap : _wraps)
	{
		if (wrap.get_name() == name)
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}

Wrap* minty::Wrapper::find_by_name(Path const& path)
{
	for (Wrap& wrap : _wraps)
	{
		if (wrap.exists(path))
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}
