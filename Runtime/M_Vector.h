#pragma once

#include "glm.hpp"
#include <string>

namespace minty
{
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::ivec2 Vector2Int;
	typedef glm::ivec3 Vector3Int;
	typedef glm::ivec4 Vector4Int;

	std::string to_string(Vector2 const& v);
	std::string to_string(Vector3 const& v);
	std::string to_string(Vector4 const& v);
	std::string to_string(Vector2Int const& v);
	std::string to_string(Vector3Int const& v);
	std::string to_string(Vector4Int const& v);
}