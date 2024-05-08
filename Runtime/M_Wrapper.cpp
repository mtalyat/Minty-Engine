#include "pch.h"
#include "M_Wrapper.h"

#include "M_Console.h"

using namespace minty;

Wrapper::Wrapper()
	: _wraps()
{}

void minty::Wrapper::emplace(Wrap const& wrap)
{
	_wraps.push_back(wrap);
}

void minty::Wrapper::emplace(Path const& path)
{
	emplace(Wrap(path));
}

size_t minty::Wrapper::get_wrap_count() const
{
	return _wraps.size();
}

Wrap& minty::Wrapper::get_wrap(size_t const index)
{
	return _wraps.at(index);
}

Wrap const& minty::Wrapper::get_wrap(size_t const index) const
{
	return _wraps.at(index);
}

Wrap* minty::Wrapper::find_by_path(Path const& path)
{
	for (Wrap& wrap : _wraps)
	{
		if (wrap.contains(path))
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}

Wrap const* minty::Wrapper::find_by_path(Path const& path) const
{
	for (Wrap const& wrap : _wraps)
	{
		if (wrap.contains(path))
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}

Wrap* minty::Wrapper::find_by_name(String const& name)
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

Wrap const* minty::Wrapper::find_by_name(String const& name) const
{
	for (Wrap const& wrap : _wraps)
	{
		if (wrap.get_name() == name)
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}

bool minty::Wrapper::contains(Path const& path) const
{
	for (Wrap const& wrap : _wraps)
	{
		if (wrap.contains(path))
		{
			return true;
		}
	}

	return false;
}

bool minty::Wrapper::open(Path const& path, VirtualFile& file) const
{
	// find wrapper
	Wrap const* wrap = find_by_path(path);

	if (!wrap) return false;

	// open file
	return wrap->open(path, file);
}

std::vector<char> minty::Wrapper::read(Path const& path) const
{
	// find wrapper
	Wrap const* wrap = find_by_path(path);

	if (!wrap) return std::vector<char>();

	// read file
	return wrap->read(path);
}
