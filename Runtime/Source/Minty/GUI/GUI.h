#pragma once

#include "Minty/Core/Base.h"
#include "Minty/Core/Direction.h"
#include "Minty/GUI/Enums.h"
#include "Minty/GUI/Theme.h"
#include "Minty/Input/Key.h"
#include "Minty/Input/Mouse.h"
#include "Minty/Render/RenderPass.h"
#include "Minty/Render/RenderTarget.h"
#include "Minty/Serialize/Node.h"

namespace Minty
{
    struct GuiViewport
    {
        Handle id;
        GuiViewportFlags flags;
        Float2 position;
        Float2 size;
    };

	struct GUIBuilder
	{

	};

    struct GuiFileDialogBuilder
    {
        Path path;
    };

	/// <summary>
	/// Provides a way to easily create a Graphical User Interface. Mostly used for debugging.
	/// </summary>
	class GUI
	{
    private:
        static Owner<RenderPass> s_renderPass;
        static Ref<RenderTarget> s_renderTarget;

	private:
		GUI() = default;
		~GUI() = default;

	public:
		static void initialize(GUIBuilder const& builder);
		static void shutdown();

#pragma region Resources

    public:
        static Ref<RenderPass> get_render_pass() { return s_renderPass.create_ref(); }

        static Ref<RenderTarget> get_render_target() { return s_renderTarget; }

#pragma endregion

#pragma region Rendering

        static Int start_frame();
        static void end_frame();

        static void start_pass();
        static void end_pass();

#pragma endregion

#pragma region Core

        static Bool begin(String const& name, Bool* const open = nullptr, GuiWindowFlags const flags = GuiWindowFlags::None);

        static void end();

        static Bool begin_child(String const& id, Float2 const& size, Bool const border = false, GuiWindowFlags const flags = GuiWindowFlags::None);

        static void end_child();

#pragma endregion

#pragma region Cursor

        static Float2 get_cursor_position();

        static Float get_cursor_position_x();

        static Float get_cursor_position_y();

        static void set_cursor_position(Float2 const& pos);

        static void set_cursor_position_x(Float const x);

        static void set_cursor_position_y(Float const y);

#pragma endregion

#pragma region Disabled

        static void begin_disabled(Bool const disabled = true);

        static void end_disabled();

#pragma endregion

#pragma region Docking

        static void dock_space(String const& id, Float2 const& size = Float2(), GuiDockFlags const flags = GuiDockFlags::None);

#pragma endregion

#pragma region Elements

        static Bool button(String const& label, Float2 const& size = Float2());

        static Bool selectable(String const& label, Bool const selected, GuiSelectableFlags const flags = GuiSelectableFlags::None, Float2 const& size = Float2());

        static Bool selectable(String const& label, Bool& selected, GuiSelectableFlags const flags = GuiSelectableFlags::None, Float2 const& size = Float2());

        static Bool checkbox(String const& label, Bool& value);

        static void text(String const& text);

        static void text_wrapped(String const& text);

        static Bool input_text(String const& label, Char* const buffer, Size const bufferSize, GuiInputTextFlags const flags = GuiInputTextFlags::None);

        static void node(Node const& rootNode, Bool const showRootNode = true, Bool const recursive = true);

        static Bool input_node(Node& rootNode, Bool const showRootNode = true, Size const offset = 0, Bool const recursive = true);

        static Bool input_text_expand(String const& label, Char* const buffer, Size const bufferSize, Float const widthPercent = 1.0f, GuiInputTextFlags const flags = GuiInputTextFlags::None);

        static Bool input_text_expand_offset(String const& label, Char* const buffer, Size const bufferSize, Float const offsetLeft, Float const offsetRight, Float const widthPercent = 1.0f, GuiInputTextFlags const flags = GuiInputTextFlags::None);

        static Bool input_text_multiline_expand(String const& label, Char* const buffer, Size const bufferSize, Float const widthPercent = 1.0f, GuiInputTextFlags const flags = GuiInputTextFlags::None);

        static Bool input_text_multiline_expand_offset(String const& label, Char* const buffer, Size const bufferSize, Float const offsetLeft, Float const offsetRight, Float const widthPercent = 1.0f, GuiInputTextFlags const flags = GuiInputTextFlags::None);

        static Bool button_left(String const& label, Float2 const& size = Float2());

#pragma endregion

#pragma region File Dialog

        static void file_dialog_open(String const& key, String const& title, String const& filters = "", GuiFileDialogBuilder const& builder = GuiFileDialogBuilder());

        static Bool file_dialog_display(String const& key, GuiWindowFlags const flags = GuiWindowFlags::NoCollapse, Float2 const& minSize = Float2(), Float2 const& maxSize = Float2(FLT_MAX, FLT_MAX));

        static Bool file_dialog_is_ok();

        static Path file_dialog_get_path();

        static String file_dialog_get_file_name();

        static void file_dialog_close();

#pragma endregion

#pragma region Font

        static Float2 calculate_text_size(String const& text);

#pragma endregion

#pragma region Formatting

        static void same_line(Float const offsetFromStartX = 0.0f, Float const spacing = -1.0f);

        static void separator();

        static void dummy(Float2 const& size);

#pragma endregion

#pragma region Group Box

        static void begin_group_box();

        static void end_group_box(Float2 const& size = Float2(), Float2 const& margin = Float2(), Color const borderColor = Color(63, 63, 63, 255)); // TODO: use themed color

#pragma endregion

#pragma region Input

        static Bool is_key_pressed(Key const key, Bool const repeat = true);

        static Bool is_key_modifier_pressed(KeyModifiers const keyModifier, Bool const repeat = true);

        static Bool is_mouse_down(MouseButton const button);

        static Bool is_mouse_released(MouseButton const button);

        static Bool is_mouse_clicked(MouseButton const button, Bool const repeat = false);

        static Bool is_mouse_double_clicked(MouseButton const button);

        static void set_keyboard_focus_here(Int const offset = 0);

#pragma endregion

#pragma region ID

        static Handle get_id(String const& name);

#pragma endregion

#pragma region IO

        static GuiConfigFlags get_config_flags();

        static void set_config_flags(GuiConfigFlags const flags);

        static Float2 get_mouse_position();

        static Float get_mouse_wheel();

        static Float get_mouse_wheel_horizontal();

#pragma endregion

#pragma region Item

        static Bool is_item_clicked(MouseButton const button = MouseButton::Left);

        static Bool is_item_hovered(GuiHoveredFlags const flags = GuiHoveredFlags::None);

#pragma endregion

#pragma region Menu

        static Bool begin_main_menu_bar();

        static void end_main_menu_bar();

        static Bool begin_menu(String const& label, Bool const enabled = true);

        static void end_menu();

        static Bool menu_item(String const& label, String const& shortcut = "", Bool const selected = false, Bool const enabled = false);

        static Bool menu_item(String const& label, String const& shortcut, Bool* const selected, Bool const enabled = false);

#pragma endregion

#pragma region Popup

        static Bool begin_popup(String const& id, GuiWindowFlags const flags = GuiWindowFlags::None);

        static Bool begin_popup_modal(String const& name, Bool* const open = nullptr, GuiWindowFlags const flags = GuiWindowFlags::None);

        static Bool begin_popup_context_item(String const& id, GuiPopupFlags const flags = GuiPopupFlags::MouseButtonRight);

        static void end_popup();

        static void open_popup(String const& key, GuiPopupFlags const flags = GuiPopupFlags::None);

        static void close_current_popup();

#pragma endregion

#pragma region Size

        static Float2 get_content_region_size();

#pragma endregion

#pragma region Style

        static void push_style_color(GuiColorID const id, Color const color);

        static void pop_style_color(Int const count = 1);

        static void push_style_variable(GuiStyleID const id, Float const value);

        static void push_style_variable(GuiStyleID const id, Float2 const& value);

        static void pop_style_variable(Int const count = 1);

#pragma endregion

#pragma region Theme

    public:
        static Theme create_theme();

        static void apply_theme(Theme const& theme);

#pragma endregion

#pragma region Viewport

        static GuiViewport const& get_main_viewport();

#pragma endregion

#pragma region Window

        static void set_next_window_position(Float2 const& pos, GuiCondition const condition = GuiCondition::None, Float2 const& pivot = Float2());

        static void set_next_window_size(Float2 const& size, GuiCondition const condition = GuiCondition::None);

        static void set_next_window_viewport(Handle const id);

        static Bool is_window_focused(GuiFocusedFlags const flags = GuiFocusedFlags::None);

        static Bool is_window_hovered(GuiHoveredFlags const flags = GuiHoveredFlags::None);

        static Float2 get_window_position();

        static Float2 get_window_size();

#pragma endregion
	};
}