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

ID minty::rendering::RendererBuilder::emplace_texture(TextureBuilder const* const builder, std::string const& path)
{
    return _textures.emplace({ path, builder });
}

std::vector<std::pair<std::string, TextureBuilder const*>> const& minty::rendering::RendererBuilder::get_textures() const
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

ID minty::rendering::RendererBuilder::emplace_shader(ShaderBuilder const* const builder, std::string const& vertexPath, std::string const& fragmentPath)
{
    return _shaders.emplace({ { vertexPath, fragmentPath }, builder });
}

std::vector<std::pair<std::vector<std::string>, ShaderBuilder const*>> const& minty::rendering::RendererBuilder::get_shaders() const
{
    return _shaders.data();
}

ID minty::rendering::RendererBuilder::emplace_material(MaterialBuilder const* const builder, void const* const material)
{
    // get material size from shader builder
    ShaderBuilder const* shader = _shaders.at(builder->get_shader_id()).second;

    // allocate space for copy of data
    void* dst = malloc(shader->get_material_size());

    if (dst == nullptr)
    {
        console::error("Could not malloc material.");

        return ERROR_ID;
    }

    // copy over
    memcpy(dst, material, shader->get_material_size());

    // add to register
    return _materials.emplace({ dst, builder });
}

std::vector<std::pair<void const*, MaterialBuilder const*>> const& minty::rendering::RendererBuilder::get_materials() const
{
    return _materials.data();
}
