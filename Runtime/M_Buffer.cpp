#include "pch.h"
#include "M_Buffer.h"

minty::Buffer::Buffer()
	: Asset()
	, buffer()
	, memory()
	, size()
{}

minty::Buffer::Buffer(BufferBuilder const& builder, Runtime& runtime)
	: Asset(builder.id, "", runtime)
	, buffer(builder.buffer)
	, memory(builder.memory)
	, size(builder.size)
{}
