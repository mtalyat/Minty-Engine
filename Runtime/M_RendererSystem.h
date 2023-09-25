#pragma once

#include "M_System.h"
#include "M_EntityRegistry.h"

namespace minty
{
	class Renderer;

	class RendererSystem
		: public System
	{
	private:
		Renderer* _renderer;

		Entity _mainCamera = NULL_ENTITY;

	public:
		RendererSystem(Engine* const engine, EntityRegistry* const registry);

		void load() override;

		void update() override;

		//void fixed_update() override {}

		//void unload() override {}
	};
}