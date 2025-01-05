#pragma once
#include "Minty/Data/Container.h"

#include "Minty/Render/Buffer.h"

namespace Minty
{
	// a dynamic container of bytes
	class BufferContainer
		: public Container
	{
	private:
		Owner<Buffer> m_buffer;
		BufferUsage m_usage;
		Size m_capacity;
		Size m_size;

	public:
		BufferContainer(BufferUsage const usage)
			: Container()
			, m_buffer(nullptr)
			, m_usage(usage)
			, m_capacity(0)
			, m_size(0)
		{}

		BufferContainer(Size const capacity, BufferUsage const usage)
			: Container()
			, m_buffer(nullptr)
			, m_usage(usage)
			, m_capacity(0)
			, m_size(0)
		{
			reserve(capacity);
		}

		BufferContainer(BufferContainer const& other)
			: m_buffer(m_buffer->clone())
			, m_usage(m_usage)
			, m_capacity(other.m_capacity)
			, m_size(m_size)
		{}

		BufferContainer& operator=(BufferContainer const& other)
		{
			if (this != &other)
			{
				m_buffer = other.m_buffer->clone();
				m_usage = other.m_usage;
				m_capacity = other.m_capacity;
				m_size = other.m_size;
			}

			return *this;
		}

		BufferContainer(BufferContainer&& other) noexcept
			: m_buffer(std::move(other.m_buffer))
			, m_usage(std::move(other.m_usage))
			, m_capacity(std::move(other.m_capacity))
			, m_size(std::move(other.m_size))
		{}

		BufferContainer& operator=(BufferContainer&& other) noexcept
		{
			if (this != &other)
			{
				m_buffer = std::move(other.m_buffer);
				m_usage = std::move(other.m_usage);
				m_capacity = std::move(other.m_capacity);
				m_size = std::move(other.m_size);
			}

			return *this;
		}

	public:
		void set_at(void const* const data, Size const size, Size const index) override;

		Bool append(void const* const data, Size const size) override;

		void clear() override;

		void reset() override;

		Bool reserve(Size const newCapacity) override;

		Bool resize(Size const newSize) override;

		void* data() const override { return m_buffer->data(); }

		Size size() const override { return m_size; }

		Size capacity() const override { return m_capacity; }

		Ref<Buffer> get_buffer() const { return m_buffer.create_ref(); }
	};
}