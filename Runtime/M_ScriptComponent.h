#pragma once
#include "M_Component.h"

#include "M_Types.h"
#include "M_ScriptObject.h"
#include "M_Register.h"
#include <unordered_set>

namespace minty
{
	/// <summary>
	/// Holds data for any and all scripts on one entity.
	/// 
	/// Holds function data for things such as OnInit, OnDestroy, OnUpdate, etc. as well as the script names.
	/// </summary>
	struct ScriptComponent
		: public Component
	{
		Register<ScriptObject> scripts;
	};

	/// <summary>
	/// The base component type for all ScriptOnXComponents.
	/// </summary>
	struct ScriptEventComponent
		: public Component
	{
		std::unordered_set<ID> scriptIds;

		void invoke(String const& method, ScriptComponent const& script) const;
	};

	struct ScriptOnLoadComponent
		: public ScriptEventComponent
	{};

	struct ScriptOnUpdateComponent
		: public ScriptEventComponent
	{};

	struct ScriptOnUnloadComponent
		: public ScriptEventComponent
	{};

	struct ScriptOnDestroyComponent
		: public ScriptEventComponent
	{};
}