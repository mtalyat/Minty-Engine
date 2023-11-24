#pragma once

namespace minty
{
	class Mesh;
}

namespace minty::rendering
{
	class RendererBuilder;
	class ShaderBuilder;
	class ShaderPassBuilder;
}

namespace minty::basic
{
	void create_basic_renderer_builder(minty::rendering::RendererBuilder& builder);

	void create_basic_shader_builder_3d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& shaderBuilder);

	void create_basic_shader_pass_builder_3d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderPassBuilder& shaderPassBuilder);

	//void create_basic_shader_builder_2d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& shaderBuilder, minty::rendering::ShaderPassBuilder& shaderPassBuilder);

	void create_basic_shader_builder_ui(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& shaderBuilder);

	void create_basic_shader_pass_builder_ui(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderPassBuilder& shaderPassBuilder);
}