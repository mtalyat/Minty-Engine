#pragma once
#include "Minty/Types/M_Object.h"

#include "Minty/Input/M_CursorMode.h"
#include "Minty/Events/M_Event.h"
#include "Minty/Types/M_UUID.h"
#include <unordered_map>

struct GLFWwindow;
struct GLFWgamepadstate;

namespace Minty
{
	class ScriptClass;

	struct WindowBuilder
	{
		String title = MINTY_NAME;
		int x = 100;
		int y = 100;
		unsigned int width = 960;
		unsigned int height = 540;
	};

	/// <summary>
	/// A window on the screen.
	/// </summary>
	class Window
		: public Object
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

	protected:
		struct WindowData
		{
			String title;
			unsigned int width;
			unsigned int height;

			Window::EventCallbackFunction callback;
		};

		static std::unordered_map<UUID, Window*> _windows;
		static Window* _main;

		UUID _id;
		WindowData _data;

		ScriptClass const* _scriptClass;
	protected:
		Window(WindowBuilder const& builder);

	public:
		virtual ~Window() = default;

	public:
		UUID get_id() const { return _id; }

		/// <summary>
		/// Sets the title text of this Window.
		/// </summary>
		virtual void set_title(String const& title) { _data.title = title; }

		/// <summary>
		/// Gets the title text of this Window.
		/// </summary>
		String const& get_title() const { return _data.title; }

		/// <summary>
		/// Sets the icon of this Window.
		/// </summary>
		/// <param name="path"></param>
		virtual void set_icon(Path const& path) = 0;

		/// <summary>
		/// Sets the cursor mode.
		/// </summary>
		/// <param name="mode"></param>
		virtual void set_cursor_mode(CursorMode const mode) = 0;

		/// <summary>
		/// Gets the cursor mode.
		/// </summary>
		/// <returns></returns>
		virtual CursorMode get_cursor_mode() const = 0;

		/// <summary>
		/// Maximizes the Window.
		/// </summary>
		virtual void maximize() = 0;

		/// <summary>
		/// Minimizes the Window.
		/// </summary>
		virtual void minimize() = 0;

		/// <summary>
		/// Restores the maximized or minimized window to its normal state.
		/// </summary>
		virtual void restore() = 0;

		/// <summary>
		/// Checks if the Window is currently open
		/// </summary>
		/// <returns>True if the Window is not closed.</returns>
		virtual bool is_open() const = 0;

		/// <summary>
		/// Closes the window.
		/// </summary>
		virtual void close() = 0;

		/// <summary>
		/// Refreshes the window's size.
		/// </summary>
		virtual void refresh() = 0;

		/// <summary>
		/// Gets the framebuffer width of this Window.
		/// </summary>
		/// <returns></returns>
		virtual int get_frame_width() const { return get_width(); }

		/// <summary>
		/// Gets the framebuffer height of this Window.
		/// </summary>
		/// <returns></returns>
		virtual int get_frame_height() const { return get_height(); }

		/// <summary>
		/// Gets the width of this Window.
		/// </summary>
		/// <returns></returns>
		virtual int get_width() const { return _data.width; }

		/// <summary>
		/// Gets the height of this Window.
		/// </summary>
		/// <returns></returns>
		virtual int get_height() const { return _data.height; }

		/// <summary>
		/// Processes all pending Window events.
		/// </summary>
		virtual void on_update() = 0;

		/// <summary>
		/// The function to call on an event.
		/// </summary>
		/// <param name="func"></param>
		void set_event_callback(EventCallbackFunction const& func) { _data.callback = func; }

		/// <summary>
		/// Returns a pointer to the native system window.
		/// </summary>
		/// <returns></returns>
		virtual void* get_native_window() const = 0;

		static Window* get_window(UUID const id) { return _windows.at(id); }

		static Window& main() { return *_main; }

		static Owner<Window> create(WindowBuilder const& builder = {});

	protected:
		static void register_window(Window* window);
		static void unregister_window(Window* window);
	};
}

