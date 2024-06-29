#pragma once

#include "Minty/Types/M_Types.h"
#include <unordered_map>

namespace Minty
{
	enum class MetaType
	{
		None = 0,

		// C++ Types
		Bool = 1,
		Char,
		UnsignedChar = Char,
		SignedChar,
		Short,
		UnsignedShort,
		SignedShort = Short,
		Int,
		UnsignedInt,
		SignedInt = Int,
		Long,
		UnsignedLong,
		SignedLong,
		String,
		MultilineString,

		// Engine Types
		Color,
		Vector2,
		Vector2Int,
		Vector3,
		Vector3Int,
		Vector4,
		Vector4Int,
		Matrix2,
		Matrix3,
		Matrix4
	};

	class MetaClass
	{
	private:
		std::unordered_map<String, MetaType> _members;

	public:
		MetaClass() = default;
		MetaClass(std::unordered_map<String, MetaType> const& members)
			: _members(members) {}

		~MetaClass() = default;

		MetaType at(String const& member) const { return _members.at(member); }

		MetaType get(String const& member) const;
	};

	class MetaDatabase
	{
	private:
		static std::unordered_map<String, MetaClass> _classes;

	public:
		static void init();

		static void emplace(String const& name, std::unordered_map<String, MetaType> const& members)
		{
			_classes.emplace(name, MetaClass(members));
		}

		static MetaType get(String const& name, String const& member);
	};
}