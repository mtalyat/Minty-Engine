#pragma once

#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Script/ScriptEngine.h"
#include "Minty/System/SystemRegistry.h"
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Handles linking the runtime to scripting languages, the Entity registry, and anything else that needs glued together.
	/// </summary>
	class Linker
	{
	public:
		struct Data
		{
			std::unordered_map<String, Ref<ScriptClass>> types;
		};

		static Data s_data;

	private:
		Linker() = default;

	public:
		static void link();

	public:
		static void link_script(Ref<ScriptClass> const script);

		template<typename T>
		static void register_system(String const& name)
		{
			SystemRegistry::register_system<T>(name);
		}

		template<typename T>
		static void register_component(String const& namespaceName, String const& className, Bool const link)
		{
			EntityRegistry::register_component<T>(className);
			if (link)
			{
				Ref<ScriptClass> script = ScriptEngine::find_class(ScriptEngine::get_full_name(namespaceName, className));

				if (script != nullptr)
				{
					link_script(script);
				}
			}
		}

		static void register_script(Ref<ScriptClass> const script)
		{
			EntityRegistry::register_script(script);
			link_script(script);
		}
	};
}