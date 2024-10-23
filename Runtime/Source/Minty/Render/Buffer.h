#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	enum class BufferUsage
	{
		UNDEFINED = 0,
		TRANSFER = 1,
		VERTEX = 2,
		INDEX = 3,
		UNIFORM = 4
	};

	struct BufferBuilder
	{
		BufferUsage usage;
		Size size = 0; // size in bytes
		const void* data = nullptr; // initial data, if any
		Bool frequent; // if true, data changes every frame, if false, data changes not as often
	};

	class Buffer
	{
	protected:
		Buffer() = default;

	public:
		virtual ~Buffer() = default;

	public:
		/// <summary>
		/// Sets part of the data in this Buffer.
		/// </summary>
		/// <param name="data"></param>
		virtual void set_data(const void* const data) = 0;

		/// <summary>
		/// Gets part of the data in this Buffer.
		/// </summary>
		/// <param name="data"></param>
		virtual void get_data(void* const data) const = 0;

		/// <summary>
		/// Gets the size of this Buffer in bytes.
		/// </summary>
		/// <returns></returns>
		virtual Size get_size() const = 0;

		/// <summary>
		/// Flushes this Buffer data from the cache.
		/// </summary>
		virtual void flush() const = 0;

		virtual void* get_native() const = 0;

	public:
		static Owner<Buffer> create(const BufferBuilder& builder = {});
	};
}