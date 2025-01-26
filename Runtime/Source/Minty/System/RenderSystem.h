#pragma once
#include "Minty/System/System.h"

#include "Minty/Component/CameraComponent.h"
#include "Minty/Component/TransformComponent.h"
#include "Minty/Entity/Entity.h"
#include "Minty/Render/BufferCargo.h"
#include "Minty/Render/Texture.h"
#include "Minty/Render/Sprite.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/MaterialTemplate.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/Mesh.h"
#include <unordered_map>
#include <vector>

namespace Minty
{
	class RenderSystem
		: public System
	{
	private:
		Entity m_mainCameraEntity;
		BufferCargo m_instanceCargo;
		UUID m_3dSpriteGroupId;
		UUID m_uiSpriteGroupId;

		Layer m_layerMask;

	public:
		RenderSystem(Scene& scene)
			: System::System("Render", scene)
			, m_mainCameraEntity(NULL_ENTITY)
			, m_instanceCargo(BufferUsage::Vertex)
			, m_3dSpriteGroupId(m_instanceCargo.create_group())
			, m_uiSpriteGroupId(m_instanceCargo.create_group())
			, m_layerMask(LAYER_ALL)
		{}

		~RenderSystem() = default;

		void reset() override {}

		void finalize() override;

		void draw() override;

	private:
		/// <summary>
		/// Updates the Camera uniform buffer with the Camera info.
		/// </summary>
		void update_camera(CameraComponent const& camera, TransformComponent const& transform);

		/// <summary>
		/// Updates 3D mesh objects.
		/// </summary>
		void draw_3d_meshes();

		/// <summary>
		/// Updates 3D sprite objects.
		/// </summary>
		void draw_3d_sprites();

		/// <summary>
		/// Updates 3D space objects.
		/// </summary>
		void draw_3d();

		/// <summary>
		/// Updates UI space sprite objects.
		/// </summary>
		void draw_ui_sprites();

		/// <summary>
		/// Updates UI space text objects.
		/// </summary>
		void draw_ui_text();

		/// <summary>
		/// Updates UI space objects.
		/// </summary>
		void draw_ui();

		/// <summary>
		/// Draws everything in the Scene.
		/// </summary>
		void draw_scene();

	public:
		void set_main_camera(Entity const entity) { m_mainCameraEntity = entity; }

		Entity get_main_camera() const { return m_mainCameraEntity; }
	};
}