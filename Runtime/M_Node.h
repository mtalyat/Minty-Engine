#pragma once

#include "M_Types.h"
#include <string>
#include <vector>
#include <map>

namespace minty
{
	struct Node
	{
		std::string data;

		std::map<std::string, std::vector<Node>> children;

#pragma region To

		std::string const& to_string() const;

		byte to_byte(byte const defaultValue = 0) const;

		int to_int(int const defaultValue = 0) const;

		ID to_id(ID const defaultValue = 0) const;

		unsigned int to_uint(unsigned int const defaultValue = 0) const;

		size_t to_size_t(size_t const defaultValue = 0) const;

		float to_float(float const defaultValue = 0.0f) const;

		bool to_bool(bool const defaultValue = false) const;

#pragma endregion

#pragma region Find

		Node* find(std::string const& name);

		Node const* find(std::string const& name) const;

		std::vector<Node>* find_all(std::string const& name);

		std::vector<Node> const* find_all(std::string const& name) const;

#pragma endregion

#pragma region Get

		std::string const& get_string(std::string const& name, std::string const& defaultValue = "") const;

		byte get_byte(std::string const& name, byte const defaultValue = 0) const;

		int get_int(std::string const& name, int const defaultValue = 0) const;

		ID get_id(std::string const& name, ID const defaultValue = 0) const;

		unsigned int get_uint(std::string const& name, unsigned int const defaultValue = 0) const;

		size_t get_size_t(std::string const& name, size_t const defaultValue = 0) const;

		float get_float(std::string const& name, float const defaultValue = 0.0f) const;

		bool get_bool(std::string const& name, bool const defaultValue = false) const;

#pragma endregion

		void print(int const indent = 0) const;

	public:
		friend std::string to_string(Node const& value);
	};
}