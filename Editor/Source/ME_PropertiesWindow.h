#pragma once
#include "ME_EditorWindow.h"

namespace Mintye
{
	class PropertiesWindow
		: public EditorWindow
	{
	private:
		enum class TargetMode
		{
			None,
			Entity,
			Asset,
		};

	private:
		TargetMode _targetMode;

		bool _targetIsBuiltIn;
		Minty::UUID _targetId;
		Minty::Entity _targetEntity;
		Minty::Path _targetPath;

		std::vector<Minty::String> _texts;
	public:
		PropertiesWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

		void set_scene(Minty::Ref<Minty::Scene> const scene) override;

#pragma region Drawing

	private:
		bool input_node(Minty::Node& node, bool const printRoot, uint32_t const offset);

		void draw_none();

		void draw_entity();

		void draw_component(Minty::Node& node, Minty::Size const i, Minty::Ref<Minty::Scene> const scene, Minty::EntityRegistry& registry);

		void draw_asset();

#pragma endregion

#pragma region Target

	public:
		/// <summary>
		/// Removes the target shown on the window.
		/// </summary>
		void clear_target();

		/// <summary>
		/// Sets the target to show on the window.
		/// </summary>
		/// <param name="entity"></param>
		void set_target(Minty::Entity const entity);

		/// <summary>
		/// Sets the target to show on the window.
		/// </summary>
		/// <param name="asset"></param>
		void set_target(Minty::Path const& path);

#pragma endregion
	};
}