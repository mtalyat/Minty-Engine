#pragma once
#include "M_System.h"

#include "M_Types.h"

namespace minty
{
	class ScriptSystem
		: public System
	{
	private:

	public:
		ScriptSystem(Runtime& runtime, ID const sceneId);

		void load() override;

		void update() override;

		//void fixed_update() override;

		void unload() override;
	};
}