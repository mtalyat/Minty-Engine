#pragma once
#include "M_Layer.h"

#include "Minty/Scenes/M_SceneManager.h"

namespace Minty
{
	struct Time;
	class Event;

	class DefaultLayer
		: public Layer
	{
	private:
		SceneManager _sceneManager;

	public:
		DefaultLayer() = default;
		~DefaultLayer() = default;

		SceneManager& get_scene_manager() { return _sceneManager; }
		SceneManager const& get_scene_manager() const { return _sceneManager; }

		void on_attach() override;
		void on_detach() override;
		void on_update(Time const& time) override;
		void on_event(Event& event) override;
	};
}