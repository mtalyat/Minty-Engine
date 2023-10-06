#include "pch.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_Renderer.h"

using namespace minty;
using namespace minty::rendering;

RendererBuilder::RendererBuilder(Info const* const info)
    : _info(info)
    , _maxTextures(16)
    , _maxShaders(16)
    , _maxMaterials(16)
{}

Info const* RendererBuilder::get_info() const
{
    return _info;
}

ID minty::rendering::RendererBuilder::get_max_textures() const
{
    return _maxTextures;
}

ID minty::rendering::RendererBuilder::get_max_shaders() const
{
    return _maxShaders;
}

ID minty::rendering::RendererBuilder::get_max_materials() const
{
    return _maxMaterials;
}

std::vector<std::pair<std::string, TextureBuilder const*>> const& minty::rendering::RendererBuilder::get_texture_plans() const
{
    return _textures.data();
}

void minty::rendering::RendererBuilder::set_max_textures(ID const max)
{
    _maxTextures = max;
}

void minty::rendering::RendererBuilder::set_max_shaders(ID const max)
{
    _maxShaders = max;
}

void minty::rendering::RendererBuilder::set_max_materials(ID const max)
{
    _maxMaterials = max;
}

ID minty::rendering::RendererBuilder::emplace_texture_builder(TextureBuilder const& builder)
{
    return _textureBuilders.emplace(builder);
}

ID minty::rendering::RendererBuilder::plan_texture(ID const builderId, std::string const& path)
{
    return _textures.emplace(std::pair<std::string, TextureBuilder const*>(path, &_textureBuilders.at(builderId)));
}

ID minty::rendering::RendererBuilder::emplace_shader_builder(ShaderBuilder const& builder)
{
    return _shaderBuilders.emplace(builder);
}

ID minty::rendering::RendererBuilder::plan_shader(ID const builderId, std::string const& vertexPath, std::string const& fragmentPath)
{
    return _shaders.emplace(std::pair<std::vector<std::string>, ShaderBuilder const*>({ vertexPath, fragmentPath }, &_shaderBuilders.at(builderId)));
}

std::vector<ShaderBuilder> const& minty::rendering::RendererBuilder::get_shader_builders() const
{
    return _shaderBuilders.data();
}

std::vector<std::pair<std::vector<std::string>, ShaderBuilder const*>> const& minty::rendering::RendererBuilder::get_shader_plans() const
{
    return _shaders.data();
}

ID minty::rendering::RendererBuilder::emplace_material_builder(MaterialBuilder const& builder)
{
    return _materialBuilders.emplace(builder);
}

ID minty::rendering::RendererBuilder::plan_material(ID const builderId)
{
    return _materials.emplace(&_materialBuilders.at(builderId));
}

std::vector<MaterialBuilder const*> const& minty::rendering::RendererBuilder::get_material_plans() const
{
    return _materials.data();
}