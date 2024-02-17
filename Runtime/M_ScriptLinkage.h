#pragma once

namespace minty
{
	/// <summary>
	/// Handles linking the C# code back to the C++ code.
	/// </summary>
	class ScriptLinkage
	{
	public:
		/// <summary>
		/// Links all of the functions, etc. to the C++ code.
		/// </summary>
		static void Link();
	};
}