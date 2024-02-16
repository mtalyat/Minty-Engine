#pragma once
#include "M_Component.h"

#include "M_ScriptObject.h"
#include <unordered_map>

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
		std::unordered_map<String, ScriptObject> scripts;

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}