#pragma once
#include "System.h"

#include "Minty/Core/Type.h"
#include "Minty/Core/Time.h"
#include <unordered_set>

namespace Minty
{
	class ScriptSystem
		: public System
	{
	public:
		ScriptSystem(Scene& scene);

		void load() override;

		void update(Time const& time) override;

		void finalize() override;

		//void fixed_update() override;

		void unload() override;
	};
}