#pragma once
#include "components/M_Component.h"

#include "types/M_Types.h"
#include "scripting/M_ScriptObject.h"
#include "types/M_Register.h"
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
	/// Used to trigger OnLoad, OnEnable, etc. on a new Entity
	/// </summary>
	struct TriggerScriptEvents {};

	/// <summary>
	/// The base component type for all ScriptOnXComponents.
	/// </summary>
	struct ScriptEventComponent
		: public Component
	{
		std::unordered_set<ID> scriptIds;

		void invoke(ScriptComponent const& script) const;

		void invoke(ScriptComponent const& script, std::unordered_set<String> const& componentNames) const;

		void invoke(ScriptComponent const& script, String const& componentName) const;

		virtual char const* get_method_name() const = 0;
	};

	struct ScriptOnLoadComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnEnableComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnUpdateComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnDisableComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnUnloadComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnDestroyComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerEnterComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerHoverComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerExitComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerMoveComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerDownComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerUpComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};

	struct ScriptOnPointerClickComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override;
	};
}