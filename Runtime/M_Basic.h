#pragma once

namespace minty
{
	class Mesh;
}

namespace minty::rendering
{
	class RendererBuilder;
	class ShaderBuilder;
}

namespace minty::basic
{
	void create_basic_renderer_builder(minty::rendering::RendererBuilder& builder);

	void create_basic_shader_builder_3d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& builder);

	//void create_basic_shader_builder_2d(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& builder);

	void create_basic_shader_builder_ui(minty::rendering::RendererBuilder const& rendererBuilder, minty::rendering::ShaderBuilder& builder);

	void create_basic_cube(minty::Mesh& mesh);

	void create_basic_quad(minty::Mesh& mesh);
}