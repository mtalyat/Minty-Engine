#pragma once

#include "Minty/Core/Pointer.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/String.h"
#include "Minty/Core/UUID.h"
#include "Minty/Script/ScriptField.h"
#include "Minty/Script/ScriptMethod.h"
#include "Minty/Script/ScriptProperty.h"
#include <unordered_map>

namespace Minty
{
	class ScriptAssembly;

	struct ScriptClassBuilder
	{
		UUID id = {};
		Ref<ScriptAssembly> assembly;
		String namespaceName;
		String className;
	};

	/// <summary>
	/// Represents a class within a Script.
	/// </summary>
	class ScriptClass
	{
	private:
		Ref<ScriptAssembly> m_assembly;

		UUID m_id;
		String m_namespace;
		String m_class;

	protected:
		ScriptClass(ScriptClassBuilder const& builder)
			: m_assembly(builder.assembly)
			, m_id(builder.id)
			, m_namespace(builder.namespaceName)
			, m_class(builder.className)
		{}

	public:
		virtual ~ScriptClass() = default;

	public:
		Ref<ScriptAssembly> get_assembly() const { return m_assembly; }

		String const& get_namespace_name() const { return m_namespace; }

		String const& get_class_name() const { return m_class; }

		String get_full_name() const;

		UUID id() const { return m_id; }

		virtual Bool has_method(String const& name, Int const parameterCount = 0) const = 0;

		// checks if this class is a child, grandchild, etc. of the given class
		virtual Bool is_derived_from(Ref<ScriptClass> const klass) const = 0;

		virtual void* get_native() const = 0;

	public:
		static Owner<ScriptClass> create(ScriptClassBuilder const& builder);
	};
}