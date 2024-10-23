#pragma once

#include "Minty/Core/Macros.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/ToString.h"
#include "Minty/Data/ConstantContainer.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Data/StaticContainer.h"
#include "Minty/Data/Lookup.h"
#include "Minty/Data/Register.h"
#include "Minty/Data/Variable.h"
#include "Minty/File/File.h"
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

	class Writer
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
		virtual void indent() = 0;
		void indent(String const& name) { write(name); indent(); }
		virtual void outdent() = 0;
		virtual Size depth() const = 0;

	protected:
		virtual void write_empty(String const& name) = 0;
		virtual void write_bool(String const& name, const Bool& obj) = 0;
		virtual void write_bool2(String const& name, const Bool2& obj) = 0;
		virtual void write_bool3(String const& name, const Bool3& obj) = 0;
		virtual void write_bool4(String const& name, const Bool4& obj) = 0;
		virtual void write_char(String const& name, const Char& obj) = 0;
		virtual void write_byte(String const& name, const Byte& obj) = 0;
		virtual void write_short(String const& name, const Short& obj) = 0;
		virtual void write_ushort(String const& name, const UShort& obj) = 0;
		virtual void write_int(String const& name, const Int& obj) = 0;
		virtual void write_int2(String const& name, const Int2& obj) = 0;
		virtual void write_int3(String const& name, const Int3& obj) = 0;
		virtual void write_int4(String const& name, const Int4& obj) = 0;
		virtual void write_uint(String const& name, const UInt& obj) = 0;
		virtual void write_uint2(String const& name, const UInt2& obj) = 0;
		virtual void write_uint3(String const& name, const UInt3& obj) = 0;
		virtual void write_uint4(String const& name, const UInt4& obj) = 0;
		virtual void write_long(String const& name, const Long& obj) = 0;
		virtual void write_ulong(String const& name, const ULong& obj) = 0;
		virtual void write_size(String const& name, const Size& obj) = 0;
		virtual void write_float(String const& name, const Float& obj) = 0;
		virtual void write_float2(String const& name, const Float2& obj) = 0;
		virtual void write_float3(String const& name, const Float3& obj) = 0;
		virtual void write_float4(String const& name, const Float4& obj) = 0;
		virtual void write_double(String const& name, const Double& obj) = 0;
		virtual void write_string(String const& name, const String& obj) = 0;
		virtual void write_uuid(String const& name, const UUID& obj) = 0;
		virtual void write_type(String const& name, const Type& obj) = 0;
		virtual void write_variable(String const& name, const Variable& obj) = 0;

	public:
		virtual void write_raw(String const& name, const void* const data, const Size size) = 0;

		void write(String const& name, const void* const data, Type const type);

	public:
		/// <summary>
		/// Writes a Node with no value.
		/// </summary>
		/// <param name="name"></param>
		void write(String const& name) { write_empty(name); }

		template<typename T, typename std::enable_if<!std::is_base_of<Serializable, T>::value, int>::type = 0>
		void write(String const& name, const T& data)
		{
			// TODO: custom read for enums

			// default: write as string
			write(name, to_string(data));
		}

		template<typename T, typename std::enable_if<std::is_base_of<Serializable, T>::value, int>::type = 0>
		void write(String const& name, const T& data)
		{
			write_object(name, data);
		}

		template<>
		void write(String const& name, const Bool& data)
		{
			write_bool(name, data);
		}
		template<>
		void write(String const& name, const Bool2& data)
		{
			write_bool2(name, data);
		}
		template<>
		void write(String const& name, const Bool3& data)
		{
			write_bool3(name, data);
		}
		template<>
		void write(String const& name, const Bool4& data)
		{
			write_bool4(name, data);
		}
		template<>
		void write(String const& name, const Char& data)
		{
			write_char(name, data);
		}
		template<>
		void write(String const& name, const Byte& data)
		{
			write_byte(name, data);
		}
		template<>
		void write(String const& name, const Short& data)
		{
			write_short(name, data);
		}
		template<>
		void write(String const& name, const UShort& data)
		{
			write_ushort(name, data);
		}
		template<>
		void write(String const& name, const Int& data)
		{
			write_int(name, data);
		}
		template<>
		void write(String const& name, const Int2& data)
		{
			write_int2(name, data);
		}
		template<>
		void write(String const& name, const Int3& data)
		{
			write_int3(name, data);
		}
		template<>
		void write(String const& name, const Int4& data)
		{
			write_int4(name, data);
		}
		template<>
		void write(String const& name, const UInt& data)
		{
			write_uint(name, data);
		}
		template<>
		void write(String const& name, const UInt2& data)
		{
			write_uint2(name, data);
		}
		template<>
		void write(String const& name, const UInt3& data)
		{
			write_uint3(name, data);
		}
		template<>
		void write(String const& name, const UInt4& data)
		{
			write_uint4(name, data);
		}
		template<>
		void write(String const& name, const Long& data)
		{
			write_long(name, data);
		}
		template<>
		void write(String const& name, const ULong& data)
		{
			write_ulong(name, data);
		}
		template<>
		void write(String const& name, const Size& data)
		{
			write_size(name, data);
		}
		template<>
		void write(String const& name, const Float& data)
		{
			write_float(name, data);
		}
		template<>
		void write(String const& name, const Float2& data)
		{
			write_float2(name, data);
		}
		template<>
		void write(String const& name, const Float3& data)
		{
			write_float3(name, data);
		}
		template<>
		void write(String const& name, const Float4& data)
		{
			write_float4(name, data);
		}
		template<>
		void write(String const& name, const Double& data)
		{
			write_double(name, data);
		}
		template<>
		void write(String const& name, const String& data)
		{
			write_string(name, data);
		}
		template<>
		void write(String const& name, const UUID& data)
		{
			write_uuid(name, data);
		}
		template<>
		void write(String const& name, const Type& data)
		{
			write_type(name, data);
		}
		template<>
		void write(String const& name, const Variable& data)
		{
			write_variable(name, data);
		}

		template<typename T, Size U>
		void write(String const& name, const std::array<T, U>& data)
		{
			indent(name);

			// write each data as child
			for (T const& value : data)
			{
				write("", value);
			}

			outdent();
		}

		template<typename T>
		void write(String const& name, const std::vector<T>& data)
		{
			indent(name);

			// write each data as child
			for (T const& value : data)
			{
				write("", value);
			}

			outdent();
		}

		template<typename T>
		void write(String const& name, const std::unordered_set<T>& data)
		{
			indent(name);

			// write each data as child
			for (T const& value : data)
			{
				write("", value);
			}

			outdent();
		}

		template<typename T>
		void write(String const& name, const std::set<T>& data)
		{
			indent(name);

			// write each data as child
			for (T const& value : data)
			{
				write("", value);
			}

			outdent();
		}

		template<typename T, typename U>
		void write(String const& name, const std::unordered_map<T, U>& data)
		{
			indent(name);

			// write each pair as child
			// T must be able to be turned into a string
			for (auto const& [key, value] : data)
			{
				String keyString = to_string(key);
				write(keyString, value);
			}

			outdent();
		}

		template<typename T, typename U>
		void write(String const& name, const std::map<T, U>& data)
		{
			indent(name);

			// write each pair as child
			// T must be able to be turned into a string
			for (auto const& [key, value] : data)
			{
				String keyString = to_string(key);
				write(keyString, value);
			}

			outdent();
		}

		template<typename T>
		void write(String const& name, const Register<T>& data)
		{
			indent(name);

			String idString;
			for (auto const& [id, value] : data)
			{
				idString = to_string(id);
				write(idString, value);
			}

			outdent();
		}

		template<typename T>
		void write(String const& name, const Lookup<T>& data)
		{
			indent(name);

			String idString;
			for (auto const& [id, name, value] : data)
			{
				// write id and name
				write(name, id);

				// write value as a child
				indent();
				write("", value);
				outdent();
			}

			outdent();
		}

	private:
		void write_object(String const& name, Serializable const& data);
	};

#pragma endregion

#pragma region Behavior Base

	class WriterFormatBehavior
	{
	protected:
		virtual void write_indent_to_buffer(const Size indent, std::vector<Byte>& buffer) = 0;
		virtual Bool write_name_to_buffer(const String& data, std::vector<Byte>& buffer) = 0;
		virtual void write_separator_to_buffer(std::vector<Byte>& buffer) = 0;
		virtual void write_space_to_buffer(std::vector<Byte>& buffer) = 0;
		virtual void write_end_to_buffer(std::vector<Byte>& buffer) = 0;

	protected:
		virtual void write_bool_to_buffer(const Bool data, std::vector<Byte>& buffer) = 0;
		virtual void write_bool2_to_buffer(const Bool2 data, std::vector<Byte>& buffer) = 0;
		virtual void write_bool3_to_buffer(const Bool3 data, std::vector<Byte>& buffer) = 0;
		virtual void write_bool4_to_buffer(const Bool4 data, std::vector<Byte>& buffer) = 0;
		virtual void write_char_to_buffer(const Char data, std::vector<Byte>& buffer) = 0;
		virtual void write_byte_to_buffer(const Byte data, std::vector<Byte>& buffer) = 0;
		virtual void write_short_to_buffer(const Short data, std::vector<Byte>& buffer) = 0;
		virtual void write_ushort_to_buffer(const UShort data, std::vector<Byte>& buffer) = 0;
		virtual void write_int_to_buffer(const Int data, std::vector<Byte>& buffer) = 0;
		virtual void write_int2_to_buffer(const Int2 data, std::vector<Byte>& buffer) = 0;
		virtual void write_int3_to_buffer(const Int3 data, std::vector<Byte>& buffer) = 0;
		virtual void write_int4_to_buffer(const Int4 data, std::vector<Byte>& buffer) = 0;
		virtual void write_uint_to_buffer(const UInt data, std::vector<Byte>& buffer) = 0;
		virtual void write_uint2_to_buffer(const UInt2 data, std::vector<Byte>& buffer) = 0;
		virtual void write_uint3_to_buffer(const UInt3 data, std::vector<Byte>& buffer) = 0;
		virtual void write_uint4_to_buffer(const UInt4 data, std::vector<Byte>& buffer) = 0;
		virtual void write_long_to_buffer(const Long data, std::vector<Byte>& buffer) = 0;
		virtual void write_ulong_to_buffer(const ULong data, std::vector<Byte>& buffer) = 0;
		virtual void write_size_to_buffer(const Size data, std::vector<Byte>& buffer) = 0;
		virtual void write_float_to_buffer(const Float data, std::vector<Byte>& buffer) = 0;
		virtual void write_float2_to_buffer(const Float2 data, std::vector<Byte>& buffer) = 0;
		virtual void write_float3_to_buffer(const Float3 data, std::vector<Byte>& buffer) = 0;
		virtual void write_float4_to_buffer(const Float4 data, std::vector<Byte>& buffer) = 0;
		virtual void write_double_to_buffer(const Double data, std::vector<Byte>& buffer) = 0;
		virtual void write_string_to_buffer(const String& data, std::vector<Byte>& buffer) = 0;
		virtual void write_uuid_to_buffer(const UUID data, std::vector<Byte>& buffer) = 0;
		virtual void write_type_to_buffer(const Type data, std::vector<Byte>& buffer) = 0;
		virtual void write_typed_to_buffer(const Type type, void const* const data, std::vector<Byte>& buffer) = 0;
	};

	class WriterStorageBehavior
	{
	protected:
		virtual void write_data(const void* const data, const Size size) = 0;
	};

#pragma endregion

#pragma region Behaviors

	/// <summary>
	/// Reads data from a file stream.
	/// </summary>
	class FileWriterBehavior
		: private WriterStorageBehavior
	{
	private:
		File* mp_file;

	public:
		FileWriterBehavior(void* const file)
			: mp_file(static_cast<File*>(file))
		{}

		virtual ~FileWriterBehavior() = default;

	protected:
		void write_data(const void* const data, const Size size) override;
	};

	class MemoryWriterBehavior
		: private WriterStorageBehavior
	{
	private:
		Container* mp_data;

	public:
		MemoryWriterBehavior(void* const container)
			: mp_data(static_cast<Container*>(container))
		{}

		virtual ~MemoryWriterBehavior() = default;

	protected:
		void write_data(const void* const data, const Size size) override;
	};

	/// <summary>
	/// Reads plain text from the stream.
	/// </summary>
	class TextWriterBehavior
		: private WriterFormatBehavior
	{
	protected:
		void write_indent_to_buffer(const Size indent, std::vector<Byte>& buffer) override;
		Bool write_name_to_buffer(const String& data, std::vector<Byte>& buffer) override;
		void write_separator_to_buffer(std::vector<Byte>& buffer) override;
		void write_space_to_buffer(std::vector<Byte>& buffer) override;
		void write_end_to_buffer(std::vector<Byte>& buffer) override;

	protected:
		void write_bool_to_buffer(const Bool data, std::vector<Byte>& buffer) override;
		void write_bool2_to_buffer(const Bool2 data, std::vector<Byte>& buffer) override;
		void write_bool3_to_buffer(const Bool3 data, std::vector<Byte>& buffer) override;
		void write_bool4_to_buffer(const Bool4 data, std::vector<Byte>& buffer) override;
		void write_char_to_buffer(const Char data, std::vector<Byte>& buffer) override;
		void write_byte_to_buffer(const Byte data, std::vector<Byte>& buffer) override;
		void write_short_to_buffer(const Short data, std::vector<Byte>& buffer) override;
		void write_ushort_to_buffer(const UShort data, std::vector<Byte>& buffer) override;
		void write_int_to_buffer(const Int data, std::vector<Byte>& buffer) override;
		void write_int2_to_buffer(const Int2 data, std::vector<Byte>& buffer) override;
		void write_int3_to_buffer(const Int3 data, std::vector<Byte>& buffer) override;
		void write_int4_to_buffer(const Int4 data, std::vector<Byte>& buffer) override;
		void write_uint_to_buffer(const UInt data, std::vector<Byte>& buffer) override;
		void write_uint2_to_buffer(const UInt2 data, std::vector<Byte>& buffer) override;
		void write_uint3_to_buffer(const UInt3 data, std::vector<Byte>& buffer) override;
		void write_uint4_to_buffer(const UInt4 data, std::vector<Byte>& buffer) override;
		void write_long_to_buffer(const Long data, std::vector<Byte>& buffer) override;
		void write_ulong_to_buffer(const ULong data, std::vector<Byte>& buffer) override;
		void write_size_to_buffer(const Size data, std::vector<Byte>& buffer) override;
		void write_float_to_buffer(const Float data, std::vector<Byte>& buffer) override;
		void write_float2_to_buffer(const Float2 data, std::vector<Byte>& buffer) override;
		void write_float3_to_buffer(const Float3 data, std::vector<Byte>& buffer) override;
		void write_float4_to_buffer(const Float4 data, std::vector<Byte>& buffer) override;
		void write_double_to_buffer(const Double data, std::vector<Byte>& buffer) override;
		void write_string_to_buffer(const String& data, std::vector<Byte>& buffer) override;
		void write_uuid_to_buffer(const UUID data, std::vector<Byte>& buffer) override;
		void write_type_to_buffer(const Type data, std::vector<Byte>& buffer) override;
		void write_typed_to_buffer(const Type type, void const* const data, std::vector<Byte>& buffer) override;
	};

	//class BinaryWriterBehavior
	//	: private WriterFormatBehavior
	//{
	//	
	//};

#pragma endregion

#pragma region Implementation

	template<typename FormatBehavior, typename StorageBehavior>
	class WriterImplementation
		: public Writer, private FormatBehavior, private StorageBehavior
	{
	protected:
		Size m_depth;

	public:
		WriterImplementation(void* const source)
			: FormatBehavior(), StorageBehavior(source)
			, m_depth(0)
		{}

		virtual ~WriterImplementation() = default;

#pragma region Formatting

		// format implementation
	public:
		void indent() override
		{
			// increase depth
			m_depth += 1;
		}
		void outdent() override
		{
			MINTY_ASSERT(m_depth > 0);

			// decrease indent
			m_depth -= 1;
		}
		Size depth() const override { return m_depth; }

#pragma endregion

#pragma region Write

	protected:
		void write_empty(String const& name) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			this->write_name_to_buffer(name, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_bool(String const& name, const Bool& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_bool_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_bool2(String const& name, const Bool2& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_bool2_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_bool3(String const& name, const Bool3& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_bool3_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_bool4(String const& name, const Bool4& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_bool4_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_char(String const& name, const Char& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_char_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_byte(String const& name, const Byte& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_byte_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_short(String const& name, const Short& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_short_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_ushort(String const& name, const UShort& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_ushort_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_int(String const& name, const Int& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_int_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_int2(String const& name, const Int2& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_int2_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_int3(String const& name, const Int3& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_int3_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_int4(String const& name, const Int4& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_int4_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_uint(String const& name, const UInt& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_uint_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_uint2(String const& name, const UInt2& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_uint2_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_uint3(String const& name, const UInt3& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_uint3_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_uint4(String const& name, const UInt4& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_uint4_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_long(String const& name, const Long& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_long_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_ulong(String const& name, const ULong& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_ulong_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_size(String const& name, const Size& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_size_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_float(String const& name, const Float& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_float_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_float2(String const& name, const Float2& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_float2_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_float3(String const& name, const Float3& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_float3_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_float4(String const& name, const Float4& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_float4_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_double(String const& name, const Double& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_double_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_string(String const& name, const String& obj) override
		{
			if (obj.empty())
			{
				write_empty(name);
				return;
			}

			String fixedObj = replace(obj, "\n", "\\n");

			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_string_to_buffer(fixedObj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_uuid(String const& name, const UUID& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_uuid_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_type(String const& name, const Type& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			this->write_type_to_buffer(obj, buffer);
			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

		void write_variable(String const& name, const Variable& obj) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer))
			{
				this->write_separator_to_buffer(buffer);
			}
			// "type value"
			this->write_type_to_buffer(obj.type(), buffer);
			this->write_space_to_buffer(buffer);
			this->write_typed_to_buffer(obj.type(), obj.data(), buffer);

			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}
	public:
		void write_raw(String const& name, const void* const data, const Size size) override
		{
			// write to memory buffer
			std::vector<Byte> buffer;
			buffer.reserve(256);
			this->write_indent_to_buffer(m_depth, buffer);
			if (this->write_name_to_buffer(name, buffer)) { this->write_separator_to_buffer(buffer); }

			Size offset = buffer.size();
			buffer.resize(offset + size);
			memcpy(buffer.data() + offset, data, size);

			this->write_end_to_buffer(buffer);

			// write to stream
			this->write_data(buffer.data(), buffer.size());
		}

#pragma endregion
	};

#pragma endregion

#pragma region Writers

	using TextFileWriter = WriterImplementation<TextWriterBehavior, FileWriterBehavior>;
	using TextMemoryWriter = WriterImplementation<TextWriterBehavior, MemoryWriterBehavior>;
	// using BinaryFileWriter
	// using BinaryMemoryWriter

#pragma endregion

}