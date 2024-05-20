#pragma once

#include "systems/M_System.h"

#include "types/M_Register.h"
#include "rendering/M_Texture.h"
#include "rendering/M_TextureAtlas.h"
#include "rendering/M_Sprite.h"
#include "rendering/M_Material.h"
#include "rendering/M_MaterialTemplate.h"
#include "rendering/M_ShaderPass.h"
#include "rendering/M_Shader.h"
#include "rendering/M_Mesh.h"

#include "entities/M_Entity.h"

#include <unordered_map>
#include <vector>

namespace minty
{
	struct CameraComponent;
	struct TransformComponent;
	class Node;

	class RenderSystem
		: public System
	{
	private:
		Entity _mainCamera = NULL_ENTITY;

	public:
		RenderSystem(Runtime& engine, Scene& scene);

		~RenderSystem();

		void reset() override;

		void update() override;

		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, TransformComponent const& transform);

#pragma region Set

	public:
		/// <summary>
		/// Sets the Camera that this RenderEngine is rendering from.
		/// </summary>
		/// <param name="entity">The entity to render from.</param>
		void set_main_camera(Entity const entity);

#pragma endregion
	};
}