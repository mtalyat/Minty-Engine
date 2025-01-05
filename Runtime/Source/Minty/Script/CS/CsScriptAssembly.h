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
		MonoImage* mp_image;

	public:
		CsScriptAssembly(ScriptAssemblyBuilder const& builder);

		~CsScriptAssembly();

	public:
		void* get_native() override { return mp_assembly; }

		std::unordered_set<String> get_dependencies() const override;

	public:
		MonoAssembly* get_assembly() const { return mp_assembly; }

		MonoImage* get_image() const { return mp_image; }
	};
}