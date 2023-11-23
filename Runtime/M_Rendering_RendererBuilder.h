#pragma once
#include "M_Object.h"

#include "M_Info.h"
#include "M_Window.h"
#include "M_Register.h"

#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Rendering_ShaderPassBuilder.h"
#include "M_Rendering_MaterialTemplateBuilder.h"
#include "M_Rendering_MaterialBuilder.h"

#include <vulkan/vulkan.h>

namespace minty
{
	class Renderer;
}

namespace minty::rendering
{
	/// <summary>
	/// Allows for the easiest creation of a renderer, by loading with defaults, and having many functions for configuration.
	/// </summary>
	class RendererBuilder :
		public Object
	{
	private:
		Info const* _info;

		Register<TextureBuilder const*> _textures;

		Register<ShaderBuilder const*> _shaders;

		Register<ShaderPassBuilder const*> _shaderPasses;

		Register<MaterialTemplateBuilder const*> _materialTemplates;

		Register<MaterialBuilder const*> _materials;
	public:
		RendererBuilder(Info const* const info = nullptr);

#pragma region Get

		Info const* get_info() const;

		Register<TextureBuilder const*> const& get_texture_builders() const;

		Register<ShaderBuilder const*> const& get_shader_builders() const;

		Register<ShaderPassBuilder const*> const& get_shader_pass_builders() const;

		Register<MaterialTemplateBuilder const*> const& get_material_template_builders() const;

		Register<MaterialBuilder const*> const& get_material_builders() const;

#pragma endregion

#pragma region Set

		ID emplace_texture(TextureBuilder const& builder);

		ID emplace_shader(ShaderBuilder const& builder);

		ID emplace_shader_pass(ShaderPassBuilder const& builder);

		ID emplace_material_template(MaterialTemplateBuilder const& builder);

		ID emplace_material(MaterialBuilder const& builder);

#pragma endregion

	public:
		friend std::string to_string(RendererBuilder const& value);
	};
}