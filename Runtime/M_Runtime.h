#pragma once

#include "M_Engine.h"
#include "M_Entities.h"
#include "M_Component.h"
#include "M_System.h"
#include <functional>
#include <map>

namespace minty
{
	/// <summary>
	/// Runs a game using the Minty Engine.
	/// </summary>
	class Runtime
	{
	public:
		typedef std::function<System* (Registry&)> SystemFunc;
		typedef std::function<Component*(Registry&, Entity const)> ComponentFunc;

	private:
		// the engine that this runtime uses
		Engine _engine;

		std::map<std::string const, SystemFunc const> _systemTypes;

		std::map<std::string const, ComponentFunc const> _componentTypes;
	public:
		/// <summary>
		/// Creates a new Runtime based on the given arguments.
		/// </summary>
		Runtime(int argc, char const* argv[]);

		~Runtime();

		void register_system(std::string const& name, SystemFunc const& func);

		void register_component(std::string const& name, ComponentFunc const& func);

		System* create_system(std::string const& name, Registry& registry) const;

		Component* create_component(std::string const& name, Entity const entity, Registry& registry) const;

		Engine& get_engine();

		/// <summary>
		/// Runs the Runtime.
		/// </summary>
		/// <returns></returns>
		int run();
	};
}