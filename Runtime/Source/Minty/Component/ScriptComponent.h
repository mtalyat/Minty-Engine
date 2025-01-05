#pragma once
#include "Minty/Component/ScriptObjectComponent.h"

#include "Minty/Data/Lookup.h"
#include "Minty/Script/ScriptObject.h"

namespace Minty
{
	/// <summary>
	/// Holds data for any and all scripts on one entity.
	/// 
	/// Holds function data for things such as OnInit, OnDestroy, OnUpdate, etc. as well as the script names.
	/// </summary>
	struct ScriptComponent
		: public ScriptObjectComponent // TODO: why is this ScriptObjectComponent? Why not Component?
	{
		// a list of scripts associated with the components on this entity
		Lookup<Ref<ScriptObject>> scriptObjects;

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}
