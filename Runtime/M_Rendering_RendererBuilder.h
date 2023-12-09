#pragma once
#include "M_Base.h"

#include "M_Info.h"
#include "M_Window.h"
#include "M_Register.h"

#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Rendering_ShaderPassBuilder.h"
#include "M_Rendering_MaterialTemplateBuilder.h"
#include "M_Rendering_MaterialBuilder.h"

#include "vulkan.h"

namespace minty
{
	class Renderer;
}

namespace minty::rendering
{
	/// <summary>
	/// Allows for the easiest creation of a renderer, by loading with defaults, and having many functions for configuration.
	/// </summary>
	struct RendererBuilder
	{
		Info const* info;

		Register<TextureBuilder const*> textureBuilders;
		Register<ShaderBuilder const*> shaderBuilders;
		Register<ShaderPassBuilder const*> shaderPassBuilders;
		Register<MaterialTemplateBuilder const*> materialTemplateBuilders;
		Register<MaterialBuilder const*> materialBuilders;
	};
}