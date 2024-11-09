#pragma once
#include "Minty/System/System.h"

#include "Minty/Component/CameraComponent.h"
#include "Minty/Component/TransformComponent.h"
#include "Minty/Entity/Entity.h"
#include "Minty/Render/BufferContainer.h"
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
		Entity m_camera;
		BufferContainer m_instanceContainer;

	public:
		RenderSystem(Scene& scene)
			: System::System("Render", scene)
			, m_camera(NULL_ENTITY)
			, m_instanceContainer(BufferUsage::VERTEX)
		{}

		~RenderSystem() = default;

		void reset() override {}

		void update(Time const& time) override;

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
		void set_camera(Entity const entity) { m_camera = entity; }

		/// <summary>
		/// Gets the Camera that this RenderSystem is rendering to.
		/// </summary>
		/// <returns></returns>
		Entity get_camera() const { return m_camera; }

#pragma endregion

	};
}