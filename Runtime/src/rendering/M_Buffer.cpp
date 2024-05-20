#include "pch.h"
#include "rendering/M_Buffer.h"

#include "runtime/M_Runtime.h"
#include "rendering/M_RenderEngine.h"

minty::Buffer::Buffer()
	: Asset()
	, _buffer(VK_NULL_HANDLE)
	, _memory(VK_NULL_HANDLE)
	, _size(0)
{}

minty::Buffer::Buffer(BufferBuilder const& builder, Runtime& runtime)
	: Asset(builder.id, "", runtime)
	, _buffer(builder.buffer)
	, _memory(builder.memory)
	, _size(builder.size)
{}

minty::Buffer::~Buffer()
{
	destroy();
}

void minty::Buffer::destroy()
{
	if (!_size) return;

	RenderEngine& renderEngine = get_runtime().get_render_engine();

	vkDestroyBuffer(renderEngine.get_device(), _buffer, nullptr);
	vkFreeMemory(renderEngine.get_device(), _memory, nullptr);

	_buffer = VK_NULL_HANDLE;
	_memory = VK_NULL_HANDLE;
	_size = 0;
}

VkBuffer minty::Buffer::get_buffer() const
{
	return _buffer;
}

VkDeviceMemory minty::Buffer::get_memory() const
{
	return _memory;
}

VkDeviceSize minty::Buffer::get_size() const
{
	return _size;
}
