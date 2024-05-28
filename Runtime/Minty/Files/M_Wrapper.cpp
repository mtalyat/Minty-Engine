#include "pch.h"
#include "Minty/Files/M_Wrapper.h"

#include "Minty/Tools/M_Console.h"

using namespace Minty;

Wrapper::Wrapper()
	: _wraps()
{}

void Minty::Wrapper::emplace(Wrap const& wrap)
{
	_wraps.push_back(wrap);
}

void Minty::Wrapper::emplace(Path const& path)
{
	emplace(Wrap(path));
}

size_t Minty::Wrapper::get_wrap_count() const
{
	return _wraps.size();
}

Wrap& Minty::Wrapper::get_wrap(size_t const index)
{
	return _wraps.at(index);
}

Wrap const& Minty::Wrapper::get_wrap(size_t const index) const
{
	return _wraps.at(index);
}

Wrap* Minty::Wrapper::find_by_path(Path const& path)
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

Wrap const* Minty::Wrapper::find_by_path(Path const& path) const
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

Wrap* Minty::Wrapper::find_by_name(String const& name)
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

Wrap const* Minty::Wrapper::find_by_name(String const& name) const
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

bool Minty::Wrapper::contains(Path const& path) const
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

bool Minty::Wrapper::open(Path const& path, VirtualFile& file) const
{
	// find wrapper
	Wrap const* wrap = find_by_path(path);

	if (!wrap) return false;

	// open file
	return wrap->open(path, file);
}

std::vector<char> Minty::Wrapper::read(Path const& path) const
{
	// find wrapper
	Wrap const* wrap = find_by_path(path);

	if (!wrap) return std::vector<char>();

	// read file
	return wrap->read(path);
}
