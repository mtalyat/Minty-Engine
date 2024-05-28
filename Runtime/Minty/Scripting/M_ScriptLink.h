#pragma once

#include "Minty/Systems/M_SystemRegistry.h"
#include "Minty/Entities/M_EntityRegistry.h"

namespace Minty
{
	class ScriptLink
	{
	public:
		static void link();

		static void link_script(String const& fullName);

		static void link_script(ScriptClass const& script);

		template<class T>
		static void register_system(String const& name)
		{
			SystemRegistry::register_system<T>(name);
		}

		template<class T>
		static void register_component(String const& namespaceName, String const& className, bool const link)
		{
			EntityRegistry::register_component<T>(className);
			if (link)
			{
				link_script(ScriptEngine::get_full_name(namespaceName, className));
			}
		}

		static void register_script(String const& name)
		{
			EntityRegistry::register_script(name);
			link_script(name);
		}
	};
}