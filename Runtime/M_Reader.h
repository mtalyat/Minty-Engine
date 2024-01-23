#pragma once

#include "M_Base.h"
#include "M_Node.h"
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>

namespace minty
{
	class Object;
	class ISerializable;

	/// <summary>
	/// Reads data from a serialized Node.
	/// </summary>
	class Reader
	{
	private:
		Node const& _node;
		void const* _data;

	public:
		/// <summary>
		/// Creates a new Reader that will read from the given Node.
		/// </summary>
		/// <param name="node">The Node to read from.</param>
		/// <param name="data">The user data that can be referenced while reading.</param>
		Reader(Node const& node, void const* const data = nullptr);

		/// <summary>
		/// Gets the root node that this Reader is using.
		/// </summary>
		/// <returns>A pointer to the root node.</returns>
		Node const& get_node() const;

		/// <summary>
		/// Gets the child node with the given name from the root node.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>The child node, or null if it does not exist.</returns>
		Node const* get_node(String const& name) const;

		/// <summary>
		/// Gets the extra data given to this Reader.
		/// </summary>
		/// <returns>The extra data as a void pointer.</returns>
		void const* get_data() const;

		/// <summary>
		/// Sets the extra data given to this Reader.
		/// </summary>
		/// <param name="data">The extra data as a void pointer.</param>
		void set_data(void const* data);

		/// <summary>
		/// Checks if a child with the given name exists.
		/// </summary>
		/// <param name="name">The name to check.</param>
		/// <returns>True if a child with the given name exists.</returns>
		bool exists(String const& name) const;

#pragma region Reading

		void to_serializable(ISerializable* const value) const;

		void read_serializable(String const& name, ISerializable* const value) const;

		String to_string(String const& defaultValue = "") const;

		String read_string(String const& name, String const& defaultValue = "") const;

		int to_int(int const defaultValue = 0) const;

		int read_int(String const& name, int const defaultValue = 0) const;

		unsigned int to_uint(unsigned int const defaultValue = 0) const;

		unsigned int read_uint(String const& name, unsigned int const defaultValue = 0) const;

		ID to_id(ID const defaultValue = ERROR_ID) const;

		ID read_id(String const& name, ID const defaultValue = ERROR_ID) const;

		float to_float(float const defaultValue = 0.0f) const;

		float read_float(String const& name, float const defaultValue = 0.0f) const;

		Byte to_byte(Byte const defaultValue = 0) const;

		Byte read_byte(String const& name, Byte const defaultValue = 0) const;

		size_t to_size(size_t const defaultValue = 0) const;

		size_t read_size(String const& name, size_t const defaultValue = 0) const;

		bool to_bool(bool const defaultValue = false) const;

		bool read_bool(String const& name, bool const defaultValue = false) const;

	public:
		template<typename T>
		void to_object(T& value) const
		{
			parse_object(_node, value);
		}

		template<typename T>
		void to_object(T& value, T const& defaultValue) const
		{
			if (!parse_object(_node, value))
			{
				value = defaultValue;
			}
		}

		template<typename T>
		void read_object(String const& name, T& value) const
		{
			if (Node const* node = _node.find(name))
			{
				parse_object(*node, value);
			}
		}

		template<typename T>
		void read_object(String const& name, T& value, T const& defaultValue) const
		{
			if (Node const* node = _node.find(name))
			{
				if (parse_object(*node, value))
				{
					return;
				}
			}

			value = defaultValue;
		}

		template<typename T>
		void to_vector(std::vector<T>& value) const
		{
			parse_vector(*this, value);
		}

		template<typename T>
		void read_vector(String const& name, std::vector<T>& value) const
		{
			if (auto const* obj = _node.find(name))
			{
				parse_vector(*obj, value);
			}
			else
			{
				value.clear();
			}
		}

		template<typename T>
		void to_set(std::set<T>& value) const
		{
			parse_set(*this, value);
		}

		template<typename T>
		void read_set(String const& name, std::set<T>& value) const
		{
			if (auto const* obj = _node.find(name))
			{
				parse_set(obj, value);
			}
			else
			{
				value.clear();
			}
		}

		template<typename T>
		void to_unordered_set(std::unordered_set<T>& value) const
		{
			parse_unordered_set(*this, value);
		}

		template<typename T>
		void read_unordered_set(String const& name, std::unordered_set<T>& value) const
		{
			if (auto const* obj = _node.find(name))
			{
				parse_unordered_set(*obj, value);
			}
			else
			{
				value.clear();
			}
		}

		template<typename T, typename U>
		void to_map(std::map<T, U>& value) const
		{
			parse_map(*this, value);
		}

		template<typename T, typename U>
		void read_map(String const& name, std::map<T, U>& value) const
		{
			if (auto const* obj = _node.find(name))
			{
				parse_map(*obj, value);
			}
			else
			{
				value.clear();
			}
		}

		template<typename T, typename U>
		void to_unordered_map(std::unordered_map<T, U>& value) const
		{
			parse_unordered_map(*this, value);
		}

		template<typename T, typename U>
		void read_unordered_map(String const& name, std::unordered_map<T, U>& value) const
		{
			if (auto const* obj = _node.find(name))
			{
				parse_unordered_map(*obj, value);
			}
			else
			{
				value.clear();
			}
		}

	private:
		template<typename T>
		bool parse_object(Node const& node, T& value) const
		{
			if (Object* obj = try_cast<T, Object>(&value))
			{
				Reader reader(node);
				obj->deserialize(reader);

				return true;
			}
			else if(node.data.size())
			{
				std::stringstream stream(node.data);
				stream >> value;

				return true;
			}

			return false;
		}

		template<typename T>
		void parse_vector(Node const& node, std::vector<T>& value) const
		{
			value.clear();

			// read ordered first
			std::unordered_set<String> addedKeys;
			size_t i = 0;
			String name = std::to_string(i);
			Node const* child;

			while (child = node.find(name))
			{
				addedKeys.emplace(name);

				T t;
				parse_object(*child, t);
				value.push_back(t);

				i++;
				name = std::to_string(i);
			}

			// read everything else, add to end
			for (auto const& pair : _node.children)
			{
				if (addedKeys.contains(pair.first))
				{
					// already added in ordered code above
					continue;
				}

				// get the object
				for (auto const& childNode : pair.second)
				{
					T t;
					parse_object(childNode, t);
					value.push_back(t);
				}
			}
		}

		template<typename T>
		void parse_set(Node const& node, std::set<T>& value) const
		{
			value.clear();

			if (auto const* list = node.find_all(BLANK))
			{
				value.reserve(list->size());

				for (Node const& node : *list)
				{
					T t;
					parse_object(node, t);
					value.push_back(t);
				}
			}
		}

		template<typename T>
		void parse_unordered_set(Node const& node, std::unordered_set<T>& value) const
		{
			value.clear();

			if (auto const* list = node.find_all(BLANK))
			{
				value.reserve(list->size());

				for (Node const& node : *list)
				{
					T t;
					parse_object(node, t);
					value.push_back(t);
				}
			}
		}

		template<typename T, typename U>
		void parse_map(Node const& node, std::map<T, U>& value) const
		{
			value.clear();

			Reader reader(node, _data);

			for (auto const& child : node->children)
			{
				// parse T
				T t;
				std::stringstream stream(child.first);
				stream >> t;

				// parse U
				U u;
				reader.read_object(child.first, u);
			}
		}

		template<typename T, typename U>
		void parse_unordered_map(Node const& node, std::unordered_map<T, U>& value) const
		{
			value.clear();

			Reader reader(node, _data);

			for (auto const& child : node->children)
			{
				// parse T
				T t;
				std::stringstream stream(child.first);
				stream >> t;

				// parse U
				U u;
				reader.read_object(child.first, u);
			}
		}

#pragma endregion

	public:
		friend String to_string(Reader const& value);
	};
}