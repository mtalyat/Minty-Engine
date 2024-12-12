#pragma once
#include "ME_Minty.h"

namespace Mintye
{
	class Project;

	class BuildInfo
	{
	public:
		enum class BuildFlags
		{
			None = 0,

			/// <summary>
			/// Re-generate and re-build the program files.
			/// </summary>
			Program = 1 << 0,
			
			/// <summary>
			/// Update the application data file.
			/// </summary>
			ApplicationData = 1 << 1,

			/// <summary>
			/// Re-generate and re-build the script assembly.
			/// </summary>
			Assembly = 1 << 2,

			/// <summary>
			/// Re-build the script assembly.
			/// </summary>
			AssemblyBuild = 1 << 3,

			/// <summary>
			/// Re-compile the assets.
			/// </summary>
			Assets = 1 << 4,

			/// <summary>
			/// Re-generate, re-build and re-compile everything.
			/// </summary>
			All = 0b00011111,
		};

		friend BuildFlags operator |(BuildFlags const left, BuildFlags const right);
		friend BuildFlags operator &(BuildFlags const left, BuildFlags const right);

	private:
		BuildFlags m_buildFlags;

		bool m_release;

	public:
		BuildInfo();

#pragma region Flags

		bool get_flag(BuildFlags const flag) const;

		void set_flag(BuildFlags const flag);

		void clear_flags();

#pragma endregion

#pragma region Config

		void set_config(bool const release);

		bool is_release() const { return m_release; }
		bool is_debug() const { return !m_release; }

		Minty::String const get_config_name() const;

#pragma endregion		
	};
}