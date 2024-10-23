#pragma once
#include "CS.h"

#include "Minty/Script/ScriptAssembly.h"

namespace Minty
{
	class CsScriptAssembly
		: public ScriptAssembly
	{
	private:
		MonoAssembly* mp_assembly;

	public:
		CsScriptAssembly(ScriptAssemblyBuilder const& builder);

		~CsScriptAssembly();

	public:
		void* get_native() override { return mp_assembly; }

	public:
		MonoAssembly* get_assembly() const { return mp_assembly; }

		MonoImage* get_image() const;
	};
}