#pragma once
#include "M_Component.h"

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

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};

	/// <summary>
	/// The base component type for all ScriptOnXComponents.
	/// </summary>
	struct ScriptEventComponent
		: public Component
	{
		std::unordered_set<ID> scriptIds;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};

	struct ScriptOnLoadComponent
		: public ScriptEventComponent
	{
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};

	struct ScriptOnUpdateComponent
		: public ScriptEventComponent
	{
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};

	struct ScriptOnUnloadComponent
		: public ScriptEventComponent
	{
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}