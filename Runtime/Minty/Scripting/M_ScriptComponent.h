#pragma once
#include "Minty/Components/M_ScriptObjectComponent.h"

#include "Minty/Types/M_Types.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Types/M_Register.h"
#include <unordered_set>

namespace Minty
{
	/// <summary>
	/// Holds data for any and all scripts on one entity.
	/// 
	/// Holds function data for things such as OnInit, OnDestroy, OnUpdate, etc. as well as the script names.
	/// </summary>
	struct ScriptComponent
		: public ScriptObjectComponent
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
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONLOAD;
		}
	};

	struct ScriptOnEnableComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONENABLE;
		}
	};

	struct ScriptOnUpdateComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONUPDATE;
		}
	};

	struct ScriptOnFinalizeComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONFINALIZE;
		}
	};

	struct ScriptOnDisableComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONDISABLE;
		}
	};

	struct ScriptOnUnloadComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONUNLOAD;
		}
	};

	struct ScriptOnDestroyComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONDESTROY;
		}
	};

	struct ScriptOnPointerEnterComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERENTER;
		}
	};

	struct ScriptOnPointerHoverComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERHOVER;
		}
	};

	struct ScriptOnPointerExitComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTEREXIT;
		}
	};

	struct ScriptOnPointerMoveComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERMOVE;
		}
	};

	struct ScriptOnPointerDownComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERDOWN;
		}
	};

	struct ScriptOnPointerUpComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERUP;
		}
	};

	struct ScriptOnPointerClickComponent
		: public ScriptEventComponent
	{
		char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERCLICK;
		}
	};
}