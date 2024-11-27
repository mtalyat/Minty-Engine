#pragma once

#include "Minty/Core/Path.h"
#include "Minty/Core/Pointer.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"

#include "Minty/Script/ScriptClass.h"
#include <unordered_set>

namespace Minty
{
	struct ScriptAssemblyBuilder
	{
		Path path;
		Bool referenceOnly = false;
	};

	/// <summary>
	/// Represents an assembly that can be loaded and used.
	/// </summary>
	class ScriptAssembly
		: public Source<ScriptAssembly>
	{
	private:
		Path m_path;
		Bool m_referenceOnly;

		std::unordered_map<String, Owner<ScriptClass>> m_classesName;
		std::unordered_map<UUID, Owner<ScriptClass>> m_classesId;

	public:
		ScriptAssembly(ScriptAssemblyBuilder const& builder)
			: m_path(builder.path)
			, m_referenceOnly(builder.referenceOnly)
			, m_classesName()
			, m_classesId()
		{}

	public:
		virtual ~ScriptAssembly() {}

	public:
		virtual void* get_native() = 0;

	public:
		Path const& get_path() const { return m_path; }

		Bool is_reference_only() const { return m_referenceOnly; }

	protected:
		void emplace_class(Owner<ScriptClass> const klass);

	public:
		Ref<ScriptClass> get_class(String const& fullName) const;

		Ref<ScriptClass> get_class(UUID const id) const;

		virtual std::unordered_set<String> get_dependencies() const = 0;

	public:
		static Owner<ScriptAssembly> create(ScriptAssemblyBuilder const& builder);
	};
}