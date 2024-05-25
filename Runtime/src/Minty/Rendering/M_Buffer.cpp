#include "pch.h"
#include "Minty/Rendering/M_Buffer.h"

#include "Minty/Rendering/M_RenderEngine.h"

Minty::Buffer::~Buffer()
{
	destroy();
}

void Minty::Buffer::destroy()
{
	if (!_size) return;

	RenderEngine& renderEngine = RenderEngine::instance();

	vkDestroyBuffer(renderEngine.get_device(), _buffer, nullptr);
	vkFreeMemory(renderEngine.get_device(), _memory, nullptr);

	_buffer = VK_NULL_HANDLE;
	_memory = VK_NULL_HANDLE;
	_size = 0;
}
