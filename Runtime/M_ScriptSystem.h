#pragma once
#include "M_System.h"

#include "M_Types.h"
#include <unordered_set>

namespace minty
{
	class ScriptSystem
		: public System
	{
	public:
		ScriptSystem(Runtime& runtime, Scene& scene);

		void load() override;

		void update() override;

		//void fixed_update() override;

		void unload() override;
	};
}