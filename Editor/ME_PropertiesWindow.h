#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class PropertiesWindow
		: public EditorWindow
	{
	private:
		enum class TargetMode
		{
			None,
			Entity,
		};

	private:
		TargetMode _targetMode;
		minty::Entity _targetEntity;

	public:
		PropertiesWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void set_scene(minty::Scene* const scene) override;

#pragma region Drawing

	private:
		void draw_none() const;

		void draw_entity() const;

		void draw_component(minty::Node& node, size_t const i, minty::Scene* const scene, minty::EntityRegistry& registry) const;

#pragma endregion

#pragma region Target

	public:
		/// <summary>
		/// Sets the target to show on the window.
		/// </summary>
		/// <param name="entity"></param>
		void set_target(minty::Entity const entity);

#pragma endregion
	};
}