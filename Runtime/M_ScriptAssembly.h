#pragma once
#include "M_Object.h"

#include "M_ScriptClass.h"
#include <unordered_map>
#include <vector>

struct _MonoAssembly;
typedef struct _MonoAssembly MonoAssembly;
struct _MonoImage;
typedef struct _MonoImage MonoImage;

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
		MonoImage* _image;

		std::unordered_map<String, ScriptClass> _classes;

	public:
		ScriptAssembly(Path const& path, ScriptEngine& engine, bool const referenceOnly = false);

		~ScriptAssembly();

		String get_name() const;

		ScriptEngine& get_engine() const;

		ScriptClass const* get_class(String const& namespaceName, String const& className) const;

		ScriptClass const* get_class(String const& fullName) const;

		std::vector<ScriptClass const*> get_classes(ScriptClass const* baseClass = nullptr) const;

		ScriptClass const* search_for_class(String const& name) const;
	private:

	};
}