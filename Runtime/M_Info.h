#pragma once

#include "M_Object.h"
#include "vulkan.h"

namespace minty
{
	constexpr char const* MINTY_NAME = "Minty Engine";
	constexpr uint32_t const MINTY_VERSION = VK_MAKE_VERSION(0, 1, 0);
	constexpr uint32_t const MINTY_API_VERSION = VK_API_VERSION_1_2;

	class Info
		: public Object
	{
	private:
		String _applicationName;
		uint32_t _applicationMajor;
		uint32_t _applicationMinor;
		uint32_t _applicationPatch;

	public:
		/// <summary>
		/// Creates an empty Info.
		/// </summary>
		Info();

		/// <summary>
		/// Creates an Info.
		/// </summary>
		/// <param name="name">The application name.</param>
		/// <param name="major">The application major update.</param>
		/// <param name="minor">The application minor update.</param>
		/// <param name="patch">The application patch update.</param>
		Info(String const& name, uint32_t const major, uint32_t const minor, uint32_t const patch);

		/// <summary>
		/// Gets the name of the application.
		/// </summary>
		/// <returns>The name of the application.</returns>
		String const& get_application_name() const;

		/// <summary>
		/// Gets the application verson.
		/// </summary>
		/// <returns>The major, minor and patch all in one version uint.</returns>
		uint32_t get_application_version() const;

		/// <summary>
		/// Gets the Vulkan application info.
		/// </summary>
		/// <returns></returns>
		VkApplicationInfo get_application_info() const;

		virtual void serialize(Writer& writer) const;
		virtual void deserialize(Reader const& reader);

	public:
		friend String to_string(Info const& value);
	};
}