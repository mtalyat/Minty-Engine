#include "pch.h"
#include "Wrapper.h"

#include "Minty/Core/Console.h"

using namespace Minty;

Wrapper::Wrapper()
	: m_wraps()
{}

void Minty::Wrapper::emplace(Wrap const& wrap)
{
	m_wraps.push_back(wrap);
}

void Minty::Wrapper::emplace(Path const& path)
{
	emplace(Wrap(path));
}

Size Minty::Wrapper::get_wrap_count() const
{
	return m_wraps.size();
}

Wrap& Minty::Wrapper::get_wrap(Size const index)
{
	return m_wraps.at(index);
}

Wrap const& Minty::Wrapper::get_wrap(Size const index) const
{
	return m_wraps.at(index);
}

Wrap* Minty::Wrapper::find_by_path(Path const& path)
{
	for (Wrap& wrap : m_wraps)
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
	for (Wrap const& wrap : m_wraps)
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
	for (Wrap& wrap : m_wraps)
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
	for (Wrap const& wrap : m_wraps)
	{
		if (wrap.get_name() == name)
		{
			return &wrap;
		}
	}

	// not found
	return nullptr;
}

Bool Minty::Wrapper::contains(Path const& path) const
{
	for (Wrap const& wrap : m_wraps)
	{
		if (wrap.contains(path))
		{
			return true;
		}
	}

	return false;
}

Bool Minty::Wrapper::open(Path const& path, VirtualFile& file) const
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
