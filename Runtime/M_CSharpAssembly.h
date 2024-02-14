#pragma once
#include "M_Assembly.h"

namespace minty
{
	/// <summary>
	/// A C-Sharp assembly.
	/// </summary>
	class CSharpAssembly
		: public Assembly
	{
	private:
		MonoDomain* _rootDomain;
		MonoDomain* _appDomain;
		MonoAssembly* _assembly;

	public:
		CSharpAssembly();

		~CSharpAssembly();

		void load(Path const& path) override;
	};
}