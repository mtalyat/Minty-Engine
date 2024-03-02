#pragma once

#include "M_System.h"

#include "M_Register.h"
#include "M_Texture.h"
#include "M_TextureAtlas.h"
#include "M_Sprite.h"
#include "M_Material.h"
#include "M_MaterialTemplate.h"
#include "M_ShaderPass.h"
#include "M_Shader.h"
#include "M_Mesh.h"

#include "M_Entity.h"

#include <unordered_map>
#include <vector>
#include <unordered_set>

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

		std::unordered_set<Shader*> _shaders;

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
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}