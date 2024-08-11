#pragma once

#include "Minty/Systems/M_System.h"

#include "Minty/Types/M_Register.h"
#include "Minty/Rendering/M_Texture.h"
#include "Minty/Rendering/M_TextureAtlas.h"
#include "Minty/Rendering/M_Sprite.h"
#include "Minty/Rendering/M_Material.h"
#include "Minty/Rendering/M_MaterialTemplate.h"
#include "Minty/Rendering/M_ShaderPass.h"
#include "Minty/Rendering/M_Shader.h"
#include "Minty/Rendering/M_Mesh.h"

#include "Minty/Entities/M_Entity.h"

#include <unordered_map>
#include <vector>

namespace Minty
{
	struct CameraComponent;
	struct TransformComponent;
	class Node;

	class RenderSystem
		: public System
	{
	private:
		Entity _camera = NULL_ENTITY;

	public:
		RenderSystem(Scene& scene)
			: System::System("Render", scene) {}

		~RenderSystem() = default;

		void reset() override {}

		void update(Time const time) override;
		
		void finalize() override;

	private:
		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, TransformComponent const& transform);

#pragma region Set

	public:
		/// <summary>
		/// Sets the Camera that this RenderSystem is rendering to.
		/// </summary>
		/// <param name="entity">The entity to render from.</param>
		void set_camera(Entity const entity) { _camera = entity; }

		/// <summary>
		/// Gets the Camera that this RenderSystem is rendering to.
		/// </summary>
		/// <returns></returns>
		Entity get_camera() const { return _camera; }

#pragma endregion
	};
}