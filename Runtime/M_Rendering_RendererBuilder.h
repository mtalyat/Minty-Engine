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
	class RenderEngine;

	/// <summary>
	/// Holds data to create a new RenderEngine.
	/// </summary>
	struct RenderEngineBuilder
	{
		Info const* info;
	};
}