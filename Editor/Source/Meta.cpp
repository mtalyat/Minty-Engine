#include "Meta.h"

using namespace Minty;
using namespace Mintye;

std::unordered_map<String, MetaClass> MetaDatabase::_classes = std::unordered_map<String, MetaClass>();

Type Mintye::MetaClass::get(String const& member) const
{
	auto found = _members.find(member);

	if (found == _members.end())
	{
		return Type::Undefined;
	}

	return found->second;
}

void Mintye::MetaDatabase::initialize()
{
	MetaDatabase::emplace("Text", {
		{ "text", Type::MultilineString }
		});
}

void Mintye::MetaDatabase::shutdown()
{
	_classes.clear();
}

Type Mintye::MetaDatabase::get(String const& name, String const& member)
{
	auto found = _classes.find(name);

	if (found == _classes.end()) return Type::Undefined;

	return found->second.get(member);
}