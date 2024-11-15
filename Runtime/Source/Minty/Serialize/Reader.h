#pragma once

#include "Minty/Core/Macros.h"
#include "Minty/Core/Math.h"
#include "Minty/Core/Parse.h"
#include "Minty/Core/ToString.h"
#include "Minty/Core/Types.h"
#include "Minty/Data/ConstantContainer.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Data/StaticContainer.h"
#include "Minty/Data/Lookup.h"
#include "Minty/Data/Register.h"
#include "Minty/Data/Variable.h"
#include "Minty/File/File.h"
#include "Minty/Serialize/Node.h"
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Minty
{
	class Serializable;

#pragma region Base

	class Reader
	{
#pragma region User Data
	private:
		std::vector<void*> m_dataStack;
		std::vector<void const*> m_dataStackConst;

	public:
		void* get_data() const
		{
			if (m_dataStack.empty())
			{
				return nullptr;
			}

			return m_dataStack.back();
		}
		void pop_data() { m_dataStack.pop_back(); }
		void push_data(void* const data) { m_dataStack.push_back(data); }
		void const* get_data_const() const
		{
			if (m_dataStackConst.empty())
			{
				return nullptr;
			}

			return m_dataStackConst.back();
		}
		void pop_data_const() { m_dataStackConst.pop_back(); }
		void push_data_const(void const* const data) { m_dataStackConst.push_back(data); }
#pragma endregion

	public:
		virtual Bool indent(const Size index) = 0;
		virtual Bool indent(const String& name) = 0;
		virtual void outdent() = 0;
		virtual Size depth() const = 0;
		virtual Bool valid() const = 0;
		virtual const Node& get_current_node() const = 0;
		const String& name() const { return get_current_node().get_name(); }
		const Container& data() const { return get_current_node().get_data(); }
		virtual Owner<Reader> branch() const = 0;
		virtual Owner<Reader> branch(String const& name) const = 0;
		virtual Owner<Reader> branch(Size const index) const = 0;
		virtual Size size() const = 0;

	public:
		virtual Bool read_name(Size const index, String& obj) const = 0;
	protected:
		virtual Bool read_bool(String const& name, Bool& obj) const = 0;
		virtual Bool read_bool(Size const index, Bool& obj) const = 0;
		virtual Bool read_bool2(String const& name, Bool2& obj) const = 0;
		virtual Bool read_bool2(Size const index, Bool2& obj) const = 0;
		virtual Bool read_bool3(String const& name, Bool3& obj) const = 0;
		virtual Bool read_bool3(Size const index, Bool3& obj) const = 0;
		virtual Bool read_bool4(String const& name, Bool4& obj) const = 0;
		virtual Bool read_bool4(Size const index, Bool4& obj) const = 0;
		virtual Bool read_char(String const& name, Char& obj) const = 0;
		virtual Bool read_char(Size const index, Char& obj) const = 0;
		virtual Bool read_byte(String const& name, Byte& obj) const = 0;
		virtual Bool read_byte(Size const index, Byte& obj) const = 0;
		virtual Bool read_short(String const& name, Short& obj) const = 0;
		virtual Bool read_short(Size const index, Short& obj) const = 0;
		virtual Bool read_ushort(String const& name, UShort& obj) const = 0;
		virtual Bool read_ushort(Size const index, UShort& obj) const = 0;
		virtual Bool read_int(String const& name, Int& obj) const = 0;
		virtual Bool read_int(Size const index, Int& obj) const = 0;
		virtual Bool read_int2(String const& name, Int2& obj) const = 0;
		virtual Bool read_int2(Size const index, Int2& obj) const = 0;
		virtual Bool read_int3(String const& name, Int3& obj) const = 0;
		virtual Bool read_int3(Size const index, Int3& obj) const = 0;
		virtual Bool read_int4(String const& name, Int4& obj) const = 0;
		virtual Bool read_int4(Size const index, Int4& obj) const = 0;
		virtual Bool read_uint(String const& name, UInt& obj) const = 0;
		virtual Bool read_uint(Size const index, UInt& obj) const = 0;
		virtual Bool read_uint2(String const& name, UInt2& obj) const = 0;
		virtual Bool read_uint2(Size const index, UInt2& obj) const = 0;
		virtual Bool read_uint3(String const& name, UInt3& obj) const = 0;
		virtual Bool read_uint3(Size const index, UInt3& obj) const = 0;
		virtual Bool read_uint4(String const& name, UInt4& obj) const = 0;
		virtual Bool read_uint4(Size const index, UInt4& obj) const = 0;
		virtual Bool read_long(String const& name, Long& obj) const = 0;
		virtual Bool read_long(Size const index, Long& obj) const = 0;
		virtual Bool read_ulong(String const& name, ULong& obj) const = 0;
		virtual Bool read_ulong(Size const index, ULong& obj) const = 0;
		virtual Bool read_size(String const& name, Size& obj) const = 0;
		virtual Bool read_size(Size const index, Size& obj) const = 0;
		virtual Bool read_float(String const& name, Float& obj) const = 0;
		virtual Bool read_float(Size const index, Float& obj) const = 0;
		virtual Bool read_float2(String const& name, Float2& obj) const = 0;
		virtual Bool read_float2(Size const index, Float2& obj) const = 0;
		virtual Bool read_float3(String const& name, Float3& obj) const = 0;
		virtual Bool read_float3(Size const index, Float3& obj) const = 0;
		virtual Bool read_float4(String const& name, Float4& obj) const = 0;
		virtual Bool read_float4(Size const index, Float4& obj) const = 0;
		virtual Bool read_double(String const& name, Double& obj) const = 0;
		virtual Bool read_double(Size const index, Double& obj) const = 0;
		virtual Bool read_string(String const& name, String& obj) const = 0;
		virtual Bool read_string(Size const index, String& obj) const = 0;
		virtual Bool read_uuid(String const& name, UUID& obj) const = 0;
		virtual Bool read_uuid(Size const index, UUID& obj) const = 0;
		virtual Bool read_type(String const& name, Type& obj) const = 0;
		virtual Bool read_type(Size const index, Type& obj) const = 0;
		virtual Bool read_variable(String const& name, Variable& obj) const = 0;
		virtual Bool read_variable(Size const index, Variable& obj) const = 0;

	public:
		virtual Bool read_raw(Size const index, void* const data, const Size size) const = 0;
		virtual Bool read_raw(String const& name, void* const data, const Size size) const = 0;

		Bool read(Size const index, void* const data, Type const type) const;
		Bool read(String const& name, void* const data, Type const type) const;

		template<typename T>
		Bool read_to_container(Size const index, Container& container)
		{
			container.resize(sizeof(T));

			MINTY_ASSERT_MESSAGE(container.size() >= sizeof(T), "Cannot read to container. It is too small.");

			return read(index, *static_cast<T*>(container.data()));
		}
		template<typename T>
		Bool read_to_container(String const& name, Container& container)
		{
			container.resize(sizeof(T));

			MINTY_ASSERT_MESSAGE(container.size() >= sizeof(T), "Cannot read to container. It is too small.");

			return read(name, *static_cast<T*>(container.data()));
		}

	public:
		template<typename T, typename std::enable_if<!std::is_base_of<Serializable, T>::value, int>::type = 0>
		Bool read(String const& name, T& data)
		{
			// TODO: custom read for enums

			// default: read as string
			String stringData;
			if (read_string(name, stringData))
			{
				Parse::parse_to(stringData, data);
				return true;
			}
			return false;
		}

		template<typename T, typename std::enable_if<!std::is_base_of<Serializable, T>::value, int>::type = 0>
		Bool read(Size const index, T& data)
		{
			// TODO: custom read for enums
			
			// default: read as string
			String stringData;
			if (read_string(index, stringData))
			{
				Parse::parse_to(stringData, data);
				return true;
			}
			return false;
		}

		template<typename T, typename std::enable_if<std::is_base_of<Serializable, T>::value, int>::type = 0>
		Bool read(String const& name, T& data)
		{
			return read_object(name, data);
		}

		template<typename T, typename std::enable_if<std::is_base_of<Serializable, T>::value, int>::type = 0>
		Bool read(Size const index, T& data)
		{
			return read_object(index, data);
		}

		template<>
		Bool read(String const& name, Bool& data)
		{
			return read_bool(name, data);
		}
		template<>
		Bool read(Size const index, Bool& data)
		{
			return read_bool(index, data);
		}
		template<>
		Bool read(String const& name, Bool2& data)
		{
			return read_bool2(name, data);
		}
		template<>
		Bool read(Size const index, Bool2& data)
		{
			return read_bool2(index, data);
		}
		template<>
		Bool read(String const& name, Bool3& data)
		{
			return read_bool3(name, data);
		}
		template<>
		Bool read(Size const index, Bool3& data)
		{
			return read_bool3(index, data);
		}
		template<>
		Bool read(String const& name, Bool4& data)
		{
			return read_bool4(name, data);
		}
		template<>
		Bool read(Size const index, Bool4& data)
		{
			return read_bool4(index, data);
		}
		template<>
		Bool read(String const& name, Char& data)
		{
			return read_char(name, data);
		}
		template<>
		Bool read(Size const index, Char& data)
		{
			return read_char(index, data);
		}
		template<>
		Bool read(String const& name, Byte& data)
		{
			return read_byte(name, data);
		}
		template<>
		Bool read(Size const index, Byte& data)
		{
			return read_byte(index, data);
		}
		template<>
		Bool read(String const& name, Short& data)
		{
			return read_short(name, data);
		}
		template<>
		Bool read(Size const index, Short& data)
		{
			return read_short(index, data);
		}
		template<>
		Bool read(String const& name, UShort& data)
		{
			return read_ushort(name, data);
		}
		template<>
		Bool read(Size const index, UShort& data)
		{
			return read_ushort(index, data);
		}
		template<>
		Bool read(String const& name, Int& data)
		{
			return read_int(name, data);
		}
		template<>
		Bool read(Size const index, Int& data)
		{
			return read_int(index, data);
		}
		template<>
		Bool read(String const& name, Int2& data)
		{
			return read_int2(name, data);
		}
		template<>
		Bool read(Size const index, Int2& data)
		{
			return read_int2(index, data);
		}
		template<>
		Bool read(String const& name, Int3& data)
		{
			return read_int3(name, data);
		}
		template<>
		Bool read(Size const index, Int3& data)
		{
			return read_int3(index, data);
		}
		template<>
		Bool read(String const& name, Int4& data)
		{
			return read_int4(name, data);
		}
		template<>
		Bool read(Size const index, Int4& data)
		{
			return read_int4(index, data);
		}
		template<>
		Bool read(String const& name, UInt& data)
		{
			return read_uint(name, data);
		}
		template<>
		Bool read(Size const index, UInt& data)
		{
			return read_uint(index, data);
		}
		template<>
		Bool read(String const& name, UInt2& data)
		{
			return read_uint2(name, data);
		}
		template<>
		Bool read(Size const index, UInt2& data)
		{
			return read_uint2(index, data);
		}
		template<>
		Bool read(String const& name, UInt3& data)
		{
			return read_uint3(name, data);
		}
		template<>
		Bool read(Size const index, UInt3& data)
		{
			return read_uint3(index, data);
		}
		template<>
		Bool read(String const& name, UInt4& data)
		{
			return read_uint4(name, data);
		}
		template<>
		Bool read(Size const index, UInt4& data)
		{
			return read_uint4(index, data);
		}
		template<>
		Bool read(String const& name, Long& data)
		{
			return read_long(name, data);
		}
		template<>
		Bool read(Size const index, Long& data)
		{
			return read_long(index, data);
		}
		template<>
		Bool read(String const& name, ULong& data)
		{
			return read_ulong(name, data);
		}
		template<>
		Bool read(Size const index, ULong& data)
		{
			return read_ulong(index, data);
		}
		template<>
		Bool read(String const& name, Size& data)
		{
			return read_size(name, data);
		}
		template<>
		Bool read(Size const index, Size& data)
		{
			return read_size(index, data);
		}
		template<>
		Bool read(String const& name, Float& data)
		{
			return read_float(name, data);
		}
		template<>
		Bool read(Size const index, Float& data)
		{
			return read_float(index, data);
		}
		template<>
		Bool read(String const& name, Float2& data)
		{
			return read_float2(name, data);
		}
		template<>
		Bool read(Size const index, Float2& data)
		{
			return read_float2(index, data);
		}
		template<>
		Bool read(String const& name, Float3& data)
		{
			return read_float3(name, data);
		}
		template<>
		Bool read(Size const index, Float3& data)
		{
			return read_float3(index, data);
		}
		template<>
		Bool read(String const& name, Float4& data)
		{
			return read_float4(name, data);
		}
		template<>
		Bool read(Size const index, Float4& data)
		{
			return read_float4(index, data);
		}
		template<>
		Bool read(String const& name, Double& data)
		{
			return read_double(name, data);
		}
		template<>
		Bool read(Size const index, Double& data)
		{
			return read_double(index, data);
		}
		template<>
		Bool read(String const& name, String& data)
		{
			return read_string(name, data);
		}
		template<>
		Bool read(Size const index, String& data)
		{
			return read_string(index, data);
		}
		template<>
		Bool read(String const& name, UUID& data)
		{
			return read_uuid(name, data);
		}
		template<>
		Bool read(Size const index, UUID& data)
		{
			return read_uuid(index, data);
		}
		template<>
		Bool read(String const& name, Type& data)
		{
			return read_type(name, data);
		}
		template<>
		Bool read(Size const index, Type& data)
		{
			return read_type(index, data);
		}
		template<>
		Bool read(String const& name, Variable& variable)
		{
			return read_variable(name, variable);
		}
		template<>
		Bool read(Size const index, Variable& variable)
		{
			return read_variable(index, variable);
		}
		template<>
		Bool read(String const& name, Node& data)
		{
			// attempt to step into node
			if (indent(name))
			{
				// copy current node
				data = get_current_node();

				outdent();

				return true;
			}

			return false;
		}
		template<>
		Bool read(Size const index, Node& data)
		{
			// attempt to step into node
			if (indent(index))
			{
				// copy current node
				data = get_current_node();

				outdent();

				return true;
			}

			return false;
		}

		template<typename T, Size U>
		Bool read(String const& name, std::array<T, U>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				// fill whatever we can: min of read data and array size
				Size size = Math::min(current.get_children_count(), data.size());

				for (Size i = 0; i < size; i++)
				{
					read(i, data[i]);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T>
		Bool read(String const& name, std::vector<T>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				// resize vector so it can be populated
				Size size = current.get_children_count();
				data.resize(size);

				for (Size i = 0; i < size; i++)
				{
					read(i, data[i]);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T>
		Bool read(String const& name, std::unordered_set<T>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				Size size = current.get_children_count();
				data.clear();
				data.reserve(size);

				for (Size i = 0; i < size; i++)
				{
					T t{};
					read(i, t);
					data.emplace(t);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T>
		Bool read(String const& name, std::set<T>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				Size size = current.get_children_count();
				data.clear();

				for (Size i = 0; i < size; i++)
				{
					T t{};
					read(i, t);
					data.emplace(t);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T, typename U>
		Bool read(String const& name, std::unordered_map<T, U>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				Size size = current.get_children_count();
				data.clear();
				data.reserve(size);

				for (Size i = 0; i < size; i++)
				{
					// get node name
					String name = "";
					read_name(i, name);

					// parse the name
					T key = {};
					if (!Parse::parse_try(name, key))
					{
						MINTY_ERROR_FORMAT("Error reading unordered map. Unable to parse key from \"{}\" to type {}.", name, to_string(typeid(T)));
						continue;
					}

					if (data.contains(key))
					{
						MINTY_ERROR_FORMAT("Error reading unordered map. Duplicate key found: \"{}\".", key);
						continue;
					}

					// get node data
					U value = {};
					read(key, value);

					data.emplace(key, value);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T, typename U>
		Bool read(String const& name, std::map<T, U>& data)
		{
			if (indent(name))
			{
				Node const& current = get_current_node();

				Size size = current.get_children_count();
				data.clear();

				for (Size i = 0; i < size; i++)
				{
					// get node name
					String name = "";
					read_name(i, name);

					// parse the name
					T key = {};
					if (!Parse::parse_try(name, key))
					{
						MINTY_ERROR_FORMAT("Error reading map. Unable to parse key from \"{}\" to type {}.", name, to_string(typeid(T)));
						continue;
					}

					if (data.contains(key))
					{
						MINTY_ERROR_FORMAT("Error reading map. Duplicate key found: \"{}\".", key);
						continue;
					}

					// get node data
					U value = {};
					read(key, value);

					data.emplace(key, value);
				}

				outdent();

				return true;
			}

			return false;
		}

		template<typename T>
		Bool read(String const& name, Register<T>& data)
		{
			if (indent(name))
			{
				String idString;
				UUID id;
				T t;
				for (Size i = 0; i < size(); i++)
				{
					// read id
					read(i, idString);
					id = Parse::to_uuid(idString);

					// read value
					read(i, t);

					data.emplace(id, t);
				}

				outdent();
			}
		}

		template<typename T>
		Bool read(String const& name, Lookup<T>& data)
		{
			if (indent(name))
			{
				String name;
				UUID id;
				T t;
				for (Size i = 0; i < size(); i++)
				{
					// read name and id
					read_name(i, name);
					read(i, id);

					// get value
					indent(i);
					read(i, t);
					outdent();

					data.emplace(id, name, t);
				}

				outdent();

				return true;
			}

			return false;
		}

	private:
		Bool read_object(String const& name, Serializable& data);

		Bool read_object(Size const index, Serializable& data);
	};

#pragma endregion

#pragma region Behavior Base

	class ReaderFormatBehavior
	{
	protected:
		virtual Node read_node(const void* const data, const Size size) const = 0;

	protected:
		virtual Bool read_bool_from_buffer(const void* const data, const Size size) const = 0;
		virtual Bool2 read_bool2_from_buffer(const void* const data, const Size size) const = 0;
		virtual Bool3 read_bool3_from_buffer(const void* const data, const Size size) const = 0;
		virtual Bool4 read_bool4_from_buffer(const void* const data, const Size size) const = 0;
		virtual Char read_char_from_buffer(const void* const data, const Size size) const = 0;
		virtual Byte read_byte_from_buffer(const void* const data, const Size size) const = 0;
		virtual Short read_short_from_buffer(const void* const data, const Size size) const = 0;
		virtual UShort read_ushort_from_buffer(const void* const data, const Size size) const = 0;
		virtual Int read_int_from_buffer(const void* const data, const Size size) const = 0;
		virtual Int2 read_int2_from_buffer(const void* const data, const Size size) const = 0;
		virtual Int3 read_int3_from_buffer(const void* const data, const Size size) const = 0;
		virtual Int4 read_int4_from_buffer(const void* const data, const Size size) const = 0;
		virtual UInt read_uint_from_buffer(const void* const data, const Size size) const = 0;
		virtual UInt2 read_uint2_from_buffer(const void* const data, const Size size) const = 0;
		virtual UInt3 read_uint3_from_buffer(const void* const data, const Size size) const = 0;
		virtual UInt4 read_uint4_from_buffer(const void* const data, const Size size) const = 0;
		virtual Long read_long_from_buffer(const void* const data, const Size size) const = 0;
		virtual ULong read_ulong_from_buffer(const void* const data, const Size size) const = 0;
		virtual Size read_size_from_buffer(const void* const data, const Size size) const = 0;
		virtual Float read_float_from_buffer(const void* const data, const Size size) const = 0;
		virtual Float2 read_float2_from_buffer(const void* const data, const Size size) const = 0;
		virtual Float3 read_float3_from_buffer(const void* const data, const Size size) const = 0;
		virtual Float4 read_float4_from_buffer(const void* const data, const Size size) const = 0;
		virtual Double read_double_from_buffer(const void* const data, const Size size) const = 0;
		virtual String read_string_from_buffer(const void* const data, const Size size) const = 0;
		virtual UUID read_uuid_from_buffer(const void* const data, const Size size) const = 0;
		virtual Type read_type_from_buffer(const void* const data, const Size size) const = 0;
		virtual void* read_typed_from_buffer(const void* const data, const Size size, Type const type) const = 0;
		virtual Variable read_variable_from_buffer(const void* const data, const Size size) const = 0;
	};

	class ReaderStorageBehavior
	{
	protected:
		virtual void read_data(void* const data, const Size size) = 0;

		virtual std::vector<Byte> read_all() = 0;
	};

#pragma endregion

#pragma region Behaviors

	class NodeReaderBehavior
		: private ReaderStorageBehavior
	{
	public:
		NodeReaderBehavior(void* const ignore)
		{}

		virtual ~NodeReaderBehavior()
		{}

	protected:
		void read_data(void* const data, const Size size) override {}

		std::vector<Byte> read_all() override { return {}; }
	};

	/// <summary>
	/// Reads data from a file stream.
	/// </summary>
	class FileReaderBehavior
		: private ReaderStorageBehavior
	{
	private:
		File* mp_file;

	public:
		FileReaderBehavior(void* const file)
			: mp_file(static_cast<File*>(file))
		{}

		virtual ~FileReaderBehavior() = default;

	protected:
		void read_data(void* const data, const Size size) override;

		std::vector<Byte> read_all() override;
	};

	class MemoryReaderBehavior
		: private ReaderStorageBehavior
	{
	private:
		Container* mp_data;
		Size m_position;

	public:
		MemoryReaderBehavior(void* const container)
			: mp_data(static_cast<Container*>(container))
			, m_position(0)
		{}

		virtual ~MemoryReaderBehavior() = default;

	protected:
		void read_data(void* const data, const Size size) override;

		std::vector<Byte> read_all() override;
	};

	///// <summary>
	///// Reads data from a memory stream.
	///// </summary>
	//class MemoryReaderBehavior
	//	: private ReaderStorageBehavior
	//{
	//};

	/// <summary>
	/// Reads plain text from the stream.
	/// </summary>
	class TextReaderBehavior
		: private ReaderFormatBehavior
	{
	protected:
		Node read_node(const void* const data, const Size size) const override;

	protected:
		Bool read_bool_from_buffer(const void* const data, const Size size) const override;
		Bool2 read_bool2_from_buffer(const void* const data, const Size size) const override;
		Bool3 read_bool3_from_buffer(const void* const data, const Size size) const override;
		Bool4 read_bool4_from_buffer(const void* const data, const Size size) const override;
		Char read_char_from_buffer(const void* const data, const Size size) const override;
		Byte read_byte_from_buffer(const void* const data, const Size size) const override;
		Short read_short_from_buffer(const void* const data, const Size size) const override;
		UShort read_ushort_from_buffer(const void* const data, const Size size) const override;
		Int read_int_from_buffer(const void* const data, const Size size) const override;
		Int2 read_int2_from_buffer(const void* const data, const Size size) const override;
		Int3 read_int3_from_buffer(const void* const data, const Size size) const override;
		Int4 read_int4_from_buffer(const void* const data, const Size size) const override;
		UInt read_uint_from_buffer(const void* const data, const Size size) const override;
		UInt2 read_uint2_from_buffer(const void* const data, const Size size) const override;
		UInt3 read_uint3_from_buffer(const void* const data, const Size size) const override;
		UInt4 read_uint4_from_buffer(const void* const data, const Size size) const override;
		Long read_long_from_buffer(const void* const data, const Size size) const override;
		ULong read_ulong_from_buffer(const void* const data, const Size size) const override;
		Size read_size_from_buffer(const void* const data, const Size size) const override;
		Float read_float_from_buffer(const void* const data, const Size size) const override;
		Float2 read_float2_from_buffer(const void* const data, const Size size) const override;
		Float3 read_float3_from_buffer(const void* const data, const Size size) const override;
		Float4 read_float4_from_buffer(const void* const data, const Size size) const override;
		Double read_double_from_buffer(const void* const data, const Size size) const override;
		String read_string_from_buffer(const void* const data, const Size size) const override;
		UUID read_uuid_from_buffer(const void* const data, const Size size) const override;
		Type read_type_from_buffer(const void* const data, const Size size) const override;
		void* read_typed_from_buffer(const void* const data, const Size size, Type const type) const override;
		Variable read_variable_from_buffer(const void* const data, const Size size) const override;
	};

	//class BinaryReaderBehavior
	//	: private ReaderFormatBehavior
	//{
	//	
	//};

#pragma endregion

#pragma region Implementation

	template<typename FormatBehavior, typename StorageBehavior>
	class ReaderImplementation
		: public Reader, private FormatBehavior, private StorageBehavior
	{
	protected:
		Size m_depth;
		Node* mp_node;
		std::vector<const Node*> m_nodeStack;

	public:
		ReaderImplementation(void* const source)
			: FormatBehavior(), StorageBehavior(source)
			, m_depth(0), mp_node(new Node()), m_nodeStack()
		{
			std::vector<Byte> data = this->read_all();
			// remove any \r
			data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
			*mp_node = this->read_node(data.data(), data.size());
			m_nodeStack.push_back(mp_node);
		}

		ReaderImplementation(Node const& root)
			: FormatBehavior(), StorageBehavior(nullptr)
			, m_depth(0), mp_node(new Node()), m_nodeStack()
		{
			*mp_node = root;
			m_nodeStack.push_back(mp_node);
		}

		virtual ~ReaderImplementation()
		{
			delete mp_node;
		}

#pragma region Formatting

		// format implementation
	public:
		Bool indent(const Size index) override
		{
			// update current node, if it exists
			if (valid() && index < get_current_node().get_children_count())
			{
				m_nodeStack.push_back(&get_current_node().get_child(index));

				// increase depth
				m_depth += 1;

				return true;
			}

			return false;
		}
		Bool indent(const String& name) override
		{
			// update current node, if it exists
			if (valid() && get_current_node().has_child(name))
			{
				m_nodeStack.push_back(&get_current_node().get_child(name));

				// increase depth
				m_depth += 1;

				return true;
			}

			return false;
		}
		void outdent() override
		{
			MINTY_ASSERT(m_depth > 0);

			// change current node, if valid
			if (valid())
			{
				m_nodeStack.pop_back();
			}

			// decrease indent
			m_depth -= 1;
		}
		Size depth() const override { return m_depth; }
		Bool valid() const override { return m_depth + 1 == m_nodeStack.size(); }
		const Node& get_current_node() const override
		{
			MINTY_ASSERT_MESSAGE(valid(), "There is no current node to get. The reader has indented into a node that does not exist.");
			return *m_nodeStack.back();
		}
		Owner<Reader> branch() const override
		{
			return Owner<ReaderImplementation<FormatBehavior, NodeReaderBehavior>>(get_current_node());
		}
		Owner<Reader> branch(String const& name) const override
		{
			if (get_current_node().has_child(name))
			{
				return Owner<ReaderImplementation<FormatBehavior, NodeReaderBehavior>>(get_current_node().get_child(name));
			}

			return nullptr;
		}
		Owner<Reader> branch(Size const index) const override
		{
			if (get_current_node().has_child(index))
			{
				return Owner<ReaderImplementation<FormatBehavior, NodeReaderBehavior>>(get_current_node().get_child(index));
			}

			return nullptr;
		}
		Size size() const override
		{
			return get_current_node().get_children_count();
		}

#pragma endregion

#pragma region Read

	public:
		Bool read_name(Size const index, String& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					obj = currentNode.get_child(index).get_name();
					return true;
				}
			}

			return false;
		}

	protected:
		Bool read_bool(String const& name, Bool& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_bool_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_bool(Size const index, Bool& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_bool_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_bool2(String const& name, Bool2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_bool2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_bool2(Size const index, Bool2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_bool2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_bool3(String const& name, Bool3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_bool3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_bool3(Size const index, Bool3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_bool3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_bool4(String const& name, Bool4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_bool4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_bool4(Size const index, Bool4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_bool4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_char(String const& name, Char& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_char_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_char(Size const index, Char& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_char_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_byte(String const& name, Byte& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_byte_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_byte(Size const index, Byte& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_byte_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_short(String const& name, Short& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_short_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_short(Size const index, Short& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_short_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_ushort(String const& name, UShort& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_ushort_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_ushort(Size const index, UShort& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_ushort_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_int(String const& name, Int& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_int_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_int(Size const index, Int& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_int_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_int2(String const& name, Int2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_int2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_int2(Size const index, Int2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_int2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_int3(String const& name, Int3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_int3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_int3(Size const index, Int3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_int3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_int4(String const& name, Int4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_int4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_int4(Size const index, Int4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_int4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_uint(String const& name, UInt& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_uint_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_uint(Size const index, UInt& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_uint_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_uint2(String const& name, UInt2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_uint2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_uint2(Size const index, UInt2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_uint2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_uint3(String const& name, UInt3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_uint3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_uint3(Size const index, UInt3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_uint3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_uint4(String const& name, UInt4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_uint4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_uint4(Size const index, UInt4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_uint4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_long(String const& name, Long& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_long_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_long(Size const index, Long& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_long_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_ulong(String const& name, ULong& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_ulong_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_ulong(Size const index, ULong& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_ulong_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_size(String const& name, Size& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_size_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_size(Size const index, Size& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_size_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_float(String const& name, Float& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_float_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_float(Size const index, Float& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_float_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_float2(String const& name, Float2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_float2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_float2(Size const index, Float2& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_float2_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_float3(String const& name, Float3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_float3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_float3(Size const index, Float3& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_float3_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_float4(String const& name, Float4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_float4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_float4(Size const index, Float4& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_float4_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_double(String const& name, Double& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_double_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_double(Size const index, Double& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_double_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_string(String const& name, String& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_string_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_string(Size const index, String& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_string_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_uuid(String const& name, UUID& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_uuid_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_uuid(Size const index, UUID& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_uuid_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_type(String const& name, Type& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_type_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_type(Size const index, Type& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_type_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_variable(String const& name, Variable& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& data = currentNode.get_child(name).get_data();
					obj = this->read_variable_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}
		Bool read_variable(Size const index, Variable& obj) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& data = currentNode.get_child(index).get_data();
					obj = this->read_variable_from_buffer(data.data(), data.size());
					return true;
				}
			}
			return false;
		}

		Bool read_raw(String const& name, void* const data, const Size size) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(name))
				{
					const Container& nodeData = currentNode.get_child(name).get_data();
					memcpy(data, nodeData.data(), nodeData.size());
					return true;
				}
			}

			return false;
		}
		Bool read_raw(Size const index, void* const data, const Size size) const override
		{
			if (valid())
			{
				const Node& currentNode = get_current_node();
				if (currentNode.has_child(index))
				{
					const Container& nodeData = currentNode.get_child(index).get_data();
					memcpy(data, nodeData.data(), nodeData.size());
					return true;
				}
			}

			return false;
		}

#pragma endregion
	};

#pragma endregion

#pragma region Readers

	using TextFileReader = ReaderImplementation<TextReaderBehavior, FileReaderBehavior>;
	using TextNodeReader = ReaderImplementation<TextReaderBehavior, NodeReaderBehavior>;
	using TextMemoryReader = ReaderImplementation<TextReaderBehavior, MemoryReaderBehavior>;
	//using TextMemoryReader
	// using BinaryFileReader
	// using BinaryMemoryReader

#pragma endregion

}