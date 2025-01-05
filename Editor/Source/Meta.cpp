#include "Meta.h"

using namespace Minty;
using namespace Mintye;

std::unordered_map<String, MetaClass> MetaDatabase::m_classes = std::unordered_map<String, MetaClass>();

Type Mintye::MetaClass::get(String const& member) const
{
	auto found = m_members.find(member);

	if (found == m_members.end())
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
	m_classes.clear();
}

Type Mintye::MetaDatabase::get(String const& name, String const& member)
{
	auto found = m_classes.find(name);

	if (found == m_classes.end()) return Type::Undefined;

	return found->second.get(member);
}