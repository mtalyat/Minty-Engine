#include "pch.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_Renderer.h"

using namespace minty;
using namespace minty::rendering;

RendererBuilder::RendererBuilder(Info const* const info)
    : _info(info)
{}

Info const* RendererBuilder::get_info() const
{
    return _info;
}

ID minty::rendering::RendererBuilder::emplace_texture(TextureBuilder const& builder)
{
    return _textures.emplace(&builder);
}

ID minty::rendering::RendererBuilder::emplace_shader(ShaderBuilder const& builder)
{
    return _shaders.emplace(&builder);
}

ID minty::rendering::RendererBuilder::emplace_shader_pass(ShaderPassBuilder const& builder)
{
    return _shaderPasses.emplace(&builder);
}

ID minty::rendering::RendererBuilder::emplace_material_template(MaterialTemplateBuilder const& builder)
{
    return _materialTemplates.emplace(&builder);
}

ID minty::rendering::RendererBuilder::emplace_material(MaterialBuilder const& builder)
{
    return _materials.emplace(&builder);
}

Register<TextureBuilder const*> const& minty::rendering::RendererBuilder::get_texture_builders() const
{
    return _textures;
}

Register<ShaderBuilder const*> const& minty::rendering::RendererBuilder::get_shader_builders() const
{
    return _shaders;
}

Register<ShaderPassBuilder const*> const& minty::rendering::RendererBuilder::get_shader_pass_builders() const
{
    return _shaderPasses;
}

Register<MaterialTemplateBuilder const*> const& minty::rendering::RendererBuilder::get_material_template_builders() const
{
    return _materialTemplates;
}

Register<MaterialBuilder const*> const& minty::rendering::RendererBuilder::get_material_builders() const
{
    return _materials;
}

std::string minty::rendering::to_string(RendererBuilder const& value)
{
    return std::format("RendererBuilder()");
}
