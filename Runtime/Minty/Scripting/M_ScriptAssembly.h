#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Scripting/M_ScriptClass.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct _MonoAssembly;
typedef struct _MonoAssembly MonoAssembly;
struct _MonoImage;
typedef struct _MonoImage MonoImage;

namespace Minty
{
	class ScriptEngine;

	class ScriptAssembly
	{
		friend class ScriptEngine;
		friend class ScriptClass;

	private:
		Path _path;
		bool _refOnly;
		MonoAssembly* _assembly;
		MonoImage* _image;

		std::unordered_map<String, ScriptClass> _classes;

	public:
		ScriptAssembly(Path const& path, bool const referenceOnly = false);

		~ScriptAssembly();

		Path const& get_path() const { return _path; }

		String get_name() const { return _path.stem().string(); }

		bool is_reference_only() const { return _refOnly; }

		ScriptClass const* get_class(String const& namespaceName, String const& className) const;

		ScriptClass const* get_class(String const& fullName) const;

		std::vector<ScriptClass const*> get_classes(ScriptClass const* baseClass = nullptr) const;

		ScriptClass const* search_for_class(String const& name) const;

		std::unordered_set<String> get_dependencies() const;
	};
}