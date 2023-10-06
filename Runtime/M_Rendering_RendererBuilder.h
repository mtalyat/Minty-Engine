#pragma once
#include "M_Object.h"

#include "M_Info.h"
#include "M_Window.h"
#include "M_Register.h"

#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
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

		ID _maxTextures;
		ID _maxShaders;
		ID _maxMaterials;

		Register<TextureBuilder> _textureBuilders;
		Register<std::pair<std::string, TextureBuilder const*>> _textures;

		Register<ShaderBuilder> _shaderBuilders;
		Register<std::pair<std::vector<std::string>, ShaderBuilder const*>> _shaders;

		Register<MaterialBuilder> _materialBuilders;
		Register<MaterialBuilder const*> _materials;
	public:
		RendererBuilder(Info const* const info = nullptr);

#pragma region Get

		Info const* get_info() const;

		ID get_max_textures() const;

		ID get_max_shaders() const;

		ID get_max_materials() const;

#pragma endregion

#pragma region Set

		void set_max_textures(ID const max);

		void set_max_shaders(ID const max);

		void set_max_materials(ID const max);

#pragma endregion

#pragma region Textures

		ID emplace_texture_builder(TextureBuilder const& builder);

		ID plan_texture(ID const builderId, std::string const& path);

		std::vector<std::pair<std::string, TextureBuilder const*>> const& get_texture_plans() const;

#pragma endregion

#pragma region Shaders

		ID emplace_shader_builder(ShaderBuilder const& builder);

		ID plan_shader(ID const builderId, std::string const& vertexPath, std::string const& fragmentPath);

		std::vector<ShaderBuilder> const& get_shader_builders() const;

		std::vector<std::pair<std::vector<std::string>, ShaderBuilder const*>> const& get_shader_plans() const;

#pragma endregion

#pragma region Materials

		ID emplace_material_builder(MaterialBuilder const& builder);

		ID plan_material(ID const builderId);

		std::vector<MaterialBuilder const*> const& get_material_plans() const;

#pragma endregion

	};
}