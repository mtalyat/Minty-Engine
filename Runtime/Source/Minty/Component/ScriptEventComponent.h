#pragma once
#include "Minty/Component/ScriptComponent.h"

#include "Minty/Core/Constants.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Used to trigger OnLoad, OnEnable, etc. on a new Entity
	/// </summary>
	struct TriggerScriptEvents {};

	/// <summary>
	/// The base component type for all ScriptOnXXXComponents.
	/// </summary>
	struct ScriptEventComponent
		: public Component
	{
		// a list of event methods to call when appropriate
		Lookup<Ref<ScriptMethod>> scriptMethods;

		void invoke() const;

		void invoke(UUID const id) const;

		void invoke(String const& name) const;

		virtual Char const* get_method_name() const = 0;
	};

	struct ScriptOnLoadComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONLOAD;
		}
	};

	struct ScriptOnEnableComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONENABLE;
		}
	};

	struct ScriptOnUpdateComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONUPDATE;
		}
	};

	struct ScriptOnFinalizeComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONFINALIZE;
		}
	};

	struct ScriptOnDisableComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONDISABLE;
		}
	};

	struct ScriptOnUnloadComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONUNLOAD;
		}
	};

	struct ScriptOnDestroyComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONDESTROY;
		}
	};

	struct ScriptOnPointerEnterComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERENTER;
		}
	};

	struct ScriptOnPointerHoverComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERHOVER;
		}
	};

	struct ScriptOnPointerExitComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTEREXIT;
		}
	};

	struct ScriptOnPointerMoveComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERMOVE;
		}
	};

	struct ScriptOnPointerDownComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERDOWN;
		}
	};

	struct ScriptOnPointerUpComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERUP;
		}
	};

	struct ScriptOnPointerClickComponent
		: public ScriptEventComponent
	{
		Char const* get_method_name() const override
		{
			return SCRIPT_METHOD_NAME_ONPOINTERCLICK;
		}
	};
}
