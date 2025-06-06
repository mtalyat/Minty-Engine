#pragma once
#include "Minty/Core/M_Window.h"

#include "Minty/Libraries/M_GLFW.h"
#include <unordered_map>

namespace Minty
{
	class WindowsWindow
		: public Window
	{
	private:
		struct Gamepad
		{
			GLFWgamepadstate* state;

			char const* name;
		};

	private:
		GLFWwindow* _window;
		std::unordered_map<int, Gamepad> _gamepads;

		int _frameWidth, _frameHeight;
		int _restoreX, _restoreY;

	public:
		WindowsWindow(WindowBuilder const& builder);

		~WindowsWindow();

		void set_title(String const& title) override;
		void set_icon(Path const& path) override;
		void set_cursor_mode(CursorMode const mode) override;
		CursorMode get_cursor_mode() const override;
		void maximize() override;
		void minimize() override;
		void restore() override;
		bool is_open() const override;
		void* get_native_window() const override { return _window; }
		void close() override;
		void refresh() override;
		int get_frame_width() const override { return _frameWidth; }
		int get_frame_height() const override { return _frameHeight; }
		void on_update() override;

	private:
		void update_size();
		void save_restore_info();
	};
}