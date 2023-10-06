#pragma once

namespace minty
{
	class Mesh;
}

namespace minty::rendering
{
	class ShaderBuilder;
}

namespace minty::basic
{
	void create_basic_shader_builder(minty::rendering::ShaderBuilder& builder);

	void create_basic_cube(minty::Mesh& mesh);

	void create_basic_quad(minty::Mesh& mesh);
}