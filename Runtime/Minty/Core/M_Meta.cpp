#include "pch.h"
#include "M_Meta.h"

#include "Minty/Rendering/M_TextComponent.h"

using namespace Minty;

std::unordered_map<String, MetaClass> MetaDatabase::_classes = std::unordered_map<String, MetaClass>();

MetaType Minty::MetaClass::get(String const& member) const
{
	auto found = _members.find(member);

	if (found == _members.end())
	{
		return MetaType::None;
	}

	return found->second;
}

void Minty::MetaDatabase::init()
{
	MetaDatabase::emplace("Text", {
		{ "text", MetaType::MultilineString }
		});
}

MetaType Minty::MetaDatabase::get(String const& name, String const& member)
{
	auto found = _classes.find(name);

	if (found == _classes.end()) return MetaType::None;

	return found->second.get(member);
}
