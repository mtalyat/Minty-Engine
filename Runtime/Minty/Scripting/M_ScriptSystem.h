#pragma once
#include "Minty/Systems/M_System.h"

#include "Minty/Types/M_Types.h"
#include "Minty/Types/M_Time.h"
#include <unordered_set>

namespace Minty
{
	class ScriptSystem
		: public System
	{
	public:
		ScriptSystem(Scene& scene);

		void load() override;

		void update(Time const time) override;

		//void fixed_update() override;

		void unload() override;
	};
}