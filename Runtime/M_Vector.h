#pragma once

#include "M_Object.h"
#include "M_Types.h"
#include "glm.hpp"

namespace minty
{
	struct Vector2
		: public glm::vec2, public Object
	{
		using glm::vec2::vec;
		Vector2(glm::vec2 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector2& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector);

		friend String to_string(Vector2 const& value);
		friend Vector2 from_string_vector2(String const& value);
	};
	struct Vector3
		: public glm::vec3, public Object
	{
		using glm::vec3::vec;
		Vector3(glm::vec3 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector3& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector3& vector);

		friend String to_string(Vector3 const& value);
		friend Vector3 from_string_vector3(String const& value);
	};
	struct Vector4
		: public glm::vec4, public Object
	{
		using glm::vec4::vec;
		Vector4(glm::vec4 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector4& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector4& vector);

		friend String to_string(Vector4 const& value);
		friend Vector4 from_string_vector4(String const& value);
	};

	struct Vector2Int
		: public glm::ivec2, public Object
	{
		using glm::ivec2::vec;
		Vector2Int(glm::ivec2 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector2Int& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector2Int& vector);

		friend String to_string(Vector2Int const& value);
		friend Vector2Int from_string_vector2int(String const& value);
	};
	struct Vector3Int
		: public glm::ivec3, public Object
	{
		using glm::ivec3::vec;
		Vector3Int(glm::ivec3 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector3Int& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector3Int& vector);

		friend String to_string(Vector3Int const& value);
		friend Vector3Int from_string_vector3int(String const& value);
	};
	struct Vector4Int
		: public glm::ivec4, public Object
	{
		using glm::ivec4::vec;
		Vector4Int(glm::ivec4 const& vec);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::istream& operator>>(std::istream& stream, Vector4Int& vector);
		friend std::ostream& operator<<(std::ostream& stream, const Vector4Int& vector);

		friend String to_string(Vector4Int const& value);
		friend Vector4Int from_string_vector4int(String const& value);
	};
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