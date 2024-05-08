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
			Asset,
		};

	private:
		TargetMode _targetMode;

		bool _targetIsBuiltIn;
		minty::UUID _targetId;
		minty::Entity _targetEntity;
		minty::Path _targetPath;

		std::vector<minty::String> _texts;
	public:
		PropertiesWindow(EditorApplication& application);

		void draw() override;

		void reset() override;

		void refresh() override;

		void set_scene(minty::Scene* const scene) override;

#pragma region Drawing

	private:
		bool input_node(minty::Node& node, bool const printRoot, uint32_t const offset);

		void draw_none();

		void draw_entity();

		void draw_component(minty::Node& node, size_t const i, minty::Scene* const scene, minty::EntityRegistry& registry);

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
		void set_target(minty::Entity const entity);

		/// <summary>
		/// Sets the target to show on the window.
		/// </summary>
		/// <param name="asset"></param>
		void set_target(minty::Path const& path);

#pragma endregion
	};
}