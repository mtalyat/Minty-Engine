#pragma once

#include "M_Types.h"
#include "glm.hpp"

namespace minty
{
	typedef glm::vec2 Vector2;
	String to_string(Vector2 const& value);
	Vector2 from_string_vector2(String const& value);
	std::istream& operator>>(std::istream& stream, Vector2& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector2& vector);

	typedef glm::vec3 Vector3;
	std::istream& operator>>(std::istream& stream, Vector3& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector3& vector);
	String to_string(Vector3 const& value);
	Vector3 from_string_vector3(String const& value);

	typedef glm::vec4 Vector4;
	std::istream& operator>>(std::istream& stream, Vector4& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector4& vector);
	String to_string(Vector4 const& value);
	Vector4 from_string_vector4(String const& value);

	typedef glm::ivec2 Vector2Int;
	std::istream& operator>>(std::istream& stream, Vector2Int& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector2Int& vector);
	String to_string(Vector2Int const& value);
	Vector2Int from_string_vector2int(String const& value);

	typedef glm::ivec3 Vector3Int;
	std::istream& operator>>(std::istream& stream, Vector3Int& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector3Int& vector);
	String to_string(Vector3Int const& value);
	Vector3Int from_string_vector3int(String const& value);

	typedef glm::ivec4 Vector4Int;
	std::istream& operator>>(std::istream& stream, Vector4Int& vector);
	std::ostream& operator<<(std::ostream& stream, const Vector4Int& vector);
	String to_string(Vector4Int const& value);
	Vector4Int from_string_vector4int(String const& value);
}

//template<>
//struct std::hash<minty::Vector2>
//{
//	size_t operator()(const minty::Vector2& key) const
//	{
//		return
//			(hash<float>()(key.x)
//				^ (hash<float>()(key.y) << 1)) >> 1;
//	}
//};
//
//template<>
//struct std::hash<minty::Vector3Int>
//{
//	size_t operator()(const minty::Vector3Int& key) const
//	{
//		return (
//			(hash<int>()(key.x)
//				^ (hash<int>()(key.y) << 1)) >> 1)
//			^ (hash<int>()(key.z) << 1);
//	}
//};
//
//template<>
//struct std::hash<minty::Vector4Int>
//{
//	size_t operator()(const minty::Vector4Int& key) const
//	{
//		return (
//			(hash<int>()(key.x)
//				^ (hash<int>()(key.y) << 1)) >> 1)
//			^ (hash<int>()(key.z) << 1) ^ (hash<int>()(key.w) << 2);
//	}
//};

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