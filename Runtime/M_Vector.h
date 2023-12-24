#pragma once

#include "glm.hpp"
#include "M_Types.h"

namespace minty
{
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::ivec2 Vector2Int;
	typedef glm::ivec3 Vector3Int;
	typedef glm::ivec4 Vector4Int;

	String to_string(Vector2 const& v);
	String to_string(Vector3 const& v);
	String to_string(Vector4 const& v);
	String to_string(Vector2Int const& v);
	String to_string(Vector3Int const& v);
	String to_string(Vector4Int const& v);
}

template<>
struct std::hash<minty::Vector2Int>
{
	size_t operator()(const minty::Vector2Int& key) const
	{
		return
			(hash<int>()(key.x)
				^ (hash<int>()(key.y) << 1)) >> 1;
	}
};

template<>
struct std::hash<minty::Vector3Int>
{
	size_t operator()(const minty::Vector3Int& key) const
	{
		return (
			(hash<int>()(key.x)
				^ (hash<int>()(key.y) << 1)) >> 1)
			^ (hash<int>()(key.z) << 1);
	}
};

template<>
struct std::hash<minty::Vector4Int>
{
	size_t operator()(const minty::Vector4Int& key) const
	{
		return (
			(hash<int>()(key.x)
				^ (hash<int>()(key.y) << 1)) >> 1)
			^ (hash<int>()(key.z) << 1) ^ (hash<int>()(key.w) << 2);
	}
};