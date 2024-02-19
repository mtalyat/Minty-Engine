#pragma once
#include "M_Object.h"

#include "M_ScriptClass.h"
#include <unordered_map>
#include <vector>

struct _MonoAssembly;
typedef struct _MonoAssembly MonoAssembly;

namespace minty
{
	class ScriptEngine;

	class ScriptAssembly
	{
		friend class ScriptEngine;
		friend class ScriptClass;

	private:
		Path _path;
		ScriptEngine* _engine;
		MonoAssembly* _assembly;

		std::unordered_map<String, ScriptClass> _classes;

	public:
		ScriptAssembly(Path const& path, ScriptEngine& engine);

		void init(bool const referenceOnly = false);

		void destroy();

		String get_name() const;

		ScriptEngine& get_engine() const;

		ScriptClass const* get_class(String const& namespaceName, String const& className) const;

		ScriptClass const* get_class(String const& fullName) const;

		std::vector<ScriptClass const*> get_classes(ScriptClass const* baseClass = nullptr) const;
	private:

	};
}