#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Types/M_Register.h"
#include "Minty/Types/M_UUID.h"
#include "Minty/Types/M_Color.h"
#include "Minty/Types/M_Node.h"
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>

namespace Minty
{
	class Node;
	class Object;
	class Asset;
	class ISerializable;

	/// <summary>
	/// Writes data to a serialized Node.
	/// </summary>
	class Writer
	{
	private:
		Node* _node;
		void const* _data;

	public:
		/// <summary>
		/// Creates a new Writer that will write to the given Node.
		/// </summary>
		/// <param name="node">The Node to write to.</param>
		/// <param name="data">The user data that can be referenced while writing.</param>
		Writer(Node& node, void const* const data = nullptr);

		/// <summary>
		/// Gets the root node that this Writer is using.
		/// </summary>
		/// <returns>The root node.</returns>
		Node& get_node()
		{
			return *_node;
		}

		/// <summary>
		/// Gets the root node that this Writer is using.
		/// </summary>
		/// <returns>The root node.</returns>
		Node const& get_node() const
		{
			return *_node;
		}

		/// <summary>
		/// Gets the child node with the given name from the root node.
		/// </summary>
		/// <param name="name">The name of the child node.</param>
		/// <returns>The child node, or null if it does not exist.</returns>
		Node const* get_node(String const& name) const
		{
			return _node->find(name);
		}

		/// <summary>
		/// Gets the extra data given to this Writer.
		/// </summary>
		/// <returns>The extra data as a void pointer.</returns>
		void const* get_data() const
		{
			return _data;
		}

		/// <summary>
		/// Sets the extra data given to this Writer.
		/// </summary>
		/// <param name="data">The extra data as a void pointer.</param>
		void set_data(void const* const data)
		{
			_data = data;
		}

		/// <summary>
		/// Checks if a child with the given name exists.
		/// </summary>
		/// <param name="name">The name to check.</param>
		/// <returns>True if a child with the given name exists.</returns>
		bool exists(String const& name) const;

#pragma region Writing

		void write(String const& name);

		void write(Node const& node);

		void write(String const& name, ISerializable const& value);

		template<typename T>
		void write(String const& name, T const* value)
		{
			// write nothing if null
			if (!value)
			{
				write(Node(name));
			}
			else if (ISerializable const* serializable = try_cast<T, ISerializable const>(value))
			{
				write(name, *serializable);
			}
			else
			{
				// regular
				std::ostringstream stream;
				stream << *value;
				write(Node(name, stream.str()));
			}
		}

		template<typename T>
		void write(String const& name, T const& value)
		{
			if (ISerializable const* serializable = try_cast<T, ISerializable const>(&value))
			{
				write(name, *serializable);
			}
			else
			{
				// regular
				std::ostringstream stream;
				stream << value;
				write(Node(name, stream.str()));
			}
		}

		template<typename T>
		void write(String const& name, std::vector<T> const& value, bool ordered = false)
		{
			if (!value.size())
			{
				return;
			}

			Node node(name);
			Writer writer(node);

			if (ordered)
			{
				// ordered
				size_t i = 0;
				for (auto const& v : value)
				{
					writer.write(std::to_string(i), v);
					i++;
				}
			}
			else
			{
				// unordered
				for (auto const& v : value)
				{
					writer.write(BLANK, v);
				}
			}

			write(node);
		}

		template<typename T>
		void write(String const& name, std::set<T> const& value)
		{
			if (!value.size())
			{
				return;
			}

			Node node(name);
			Writer writer(node, _data);

			for (auto const& v : value)
			{
				writer.write(BLANK, v);
			}

			write(node);
		}

		template<typename T>
		void write(String const& name, std::unordered_set<T> const& value)
		{
			if (!value.size())
			{
				return;
			}

			Node node(name);
			Writer writer(node, _data);

			for (auto const& v : value)
			{
				writer.write(BLANK, v);
			}

			write(node);
		}

		template<typename T, typename U>
		void write(String const& name, std::map<T, U> const& value)
		{
			if (!value.size())
			{
				return;
			}

			Node node(name);
			Writer writer(node, _data);

			for (auto const& pair : value)
			{
				std::stringstream stream;
				stream << pair.first;
				writer.write(stream.str(), pair.second);
			}

			write(node);
		}

		template<typename T, typename U>
		void write(String const& name, std::unordered_map<T, U> const& value)
		{
			if (!value.size())
			{
				return;
			}

			Node node(name);
			Writer writer(node, _data);

			for (auto const& pair : value)
			{
				std::stringstream stream;
				stream << pair.first;
				writer.write(stream.str(), pair.second);
			}

			write(node);
		}

		template<typename T>
		void write(String const& name, Register<T> const& value)
		{
			for (auto const& pair : value)
			{
				write(value.get_name(pair.first), pair.second);
			}
		}

		template<typename T>
		void write_as(T const& value)
		{
			if (ISerializable const* serializable = try_cast<T, ISerializable const>(&value))
			{
				serializable->serialize(*this);
			}
			else
			{
				std::ostringstream stream;
				stream << value;
				_node->set_data(stream.str());
			}
		}

#pragma endregion

	public:
		friend String to_string(Writer const& value);
	};
}