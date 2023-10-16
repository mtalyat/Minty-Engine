#pragma once
#include "M_Rendering_Object.h"

#include "M_Rendering_MaterialBuilder.h"

#include "M_Color.h"
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace minty
{
	class Shader;

	/// <summary>
	/// Holds graphics information.
	/// </summary>
	class Material
		: public rendering::RendererObject
	{
	private:
		ID _shaderId;

		bool _dirty;

		std::unordered_map<std::string, MaterialData> _values;
	public:
		/// <summary>
		/// Creates a new Material that corresponds to the given Shader ID.
		/// </summary>
		/// <param name="shaderId">The shader ID.</param>
		/// <param name="textureId">The inital Texture ID used for this Material.</param>
		/// <param name="color">The initial Color used for this Material.</param>
		Material(rendering::MaterialBuilder const& builder, Renderer& renderer);

		void destroy();

		bool is_dirty() const;

		template<class T>
		void set(std::string const& name, T* const value);

		void set(std::string const& name, void* const value, size_t const size);

		template<class T>
		void* at(std::string const& name);

		void* get(std::string const& name);

		ID get_shader_id() const;

		/// <summary>
		/// Applies this Material's data to the Shader.
		/// </summary>
		void apply();
	};

	template<class T>
	void Material::set(std::string const& name, T* const value)
	{
		set(name, value, sizeof(T));
	}

	template<class T>
	void* Material::at(std::string const& name)
	{
		return static_cast<T*>(get(name));
	}
}