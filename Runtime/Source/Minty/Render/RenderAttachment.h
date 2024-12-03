#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Types.h"
#include "Minty/Render/Format.h"
#include "Minty/Render/Image.h"

namespace Minty
{
	enum class RenderAttachmentType
	{
		Color,
		Depth
	};

	enum class RenderAttachmentLoadOperation
	{
		Load = 0,
		Clear = 1,
		DontCare = 2,
	};

	enum class RenderAttachmentStoreOperation
	{
		Store = 0,
		DontCare = 1,
	};

	struct RenderAttachment
	{
		RenderAttachmentType type;
		Format format;
		RenderAttachmentLoadOperation loadOperation = RenderAttachmentLoadOperation::DontCare;
		RenderAttachmentStoreOperation storeOperation = RenderAttachmentStoreOperation::DontCare;
		ImageLayout initialLayout = ImageLayout::Undefined;
		ImageLayout finalLayout = ImageLayout::Undefined;
	};
}