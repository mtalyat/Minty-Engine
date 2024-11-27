#pragma once

#include "ME_Minty.h"
#include <unordered_map>

namespace Mintye
{
	class MetaClass
	{
	private:
		std::unordered_map<Minty::String, Minty::Type> _members;

	public:
		MetaClass() = default;
		MetaClass(std::unordered_map<Minty::String, Minty::Type> const& members)
			: _members(members) {}

		~MetaClass() = default;

		Minty::Type at(Minty::String const& member) const { return _members.at(member); }

		Minty::Type get(Minty::String const& member) const;
	};

	class MetaDatabase
	{
	private:
		static std::unordered_map<Minty::String, MetaClass> _classes;

	public:
		static void initialize();

		static void shutdown();

		static void emplace(Minty::String const& name, std::unordered_map<Minty::String, Minty::Type> const& members)
		{
			_classes.emplace(name, MetaClass(members));
		}

		static Minty::Type get(Minty::String const& name, Minty::String const& member);
	};
}