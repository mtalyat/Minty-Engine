#pragma once

#include "Minty/Core/Macros.h"

namespace Minty
{
    enum class GuiWindowFlags
    {
        None = 0,
        NoTitleBar = 1 << 0,   // Disable title-bar
        NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
        NoMove = 1 << 2,   // Disable user moving the window
        NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
        NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
        NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
        AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
        NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
        NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
        NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
        MenuBar = 1 << 10,  // Has a menu-bar
        HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
        NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
        NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
        AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
        AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
        AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
        NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
        NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
        UnsavedDocument = 1 << 20,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
        NoDocking = 1 << 21,  // Disable docking of this window
        NoNav = NoNavInputs | NoNavFocus,
        NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
        NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiWindowFlags);

    enum class GuiInputTextFlags
    {
        None = 0,
        CharsDecimal = 1 << 0,   // Allow 0123456789.+-*/
        CharsHexadecimal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
        CharsUppercase = 1 << 2,   // Turn a..z into A..Z
        CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
        AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
        EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
        CallbackCompletion = 1 << 6,   // Callback on pressing TAB (for completion handling)
        CallbackHistory = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
        CallbackAlways = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
        CallbackCharFilter = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
        AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
        CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
        NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
        AlwaysOverwrite = 1 << 13,  // Overwrite mode
        ReadOnly = 1 << 14,  // Read-only mode
        Password = 1 << 15,  // Password mode, display all characters as '*'
        NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
        CharsScientific = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
        CallbackResize = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
        CallbackEdit = 1 << 19,  // Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
        EscapeClearsAll = 1 << 20,  // Escape key clears content if not empty, and deactivate otherwise (contrast to default behavior of Escape to revert)
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiInputTextFlags);

    enum class GuiCondition
    {
        None = 0,        // No condition (always set the variable), same as _Always
        Always = 1 << 0,   // No condition (always set the variable), same as _None
        Once = 1 << 1,   // Set the variable once per runtime session (only the first call will succeed)
        FirstUseEver = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
        Appearing = 1 << 3,   // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiCondition);

    enum class GuiHoveredFlags
    {
        None = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
        ChildWindows = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
        RootWindow = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
        AnyWindow = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
        NoPopupHierarchy = 1 << 3,   // IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
        DockHierarchy = 1 << 4,   // IsWindowHovered() only: Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
        AllowWhenBlockedByPopup = 1 << 5,   // Return true even if a popup window is normally blocking access to this item/window
        //AllowWhenBlockedByModal     = 1 << 6,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
        AllowWhenBlockedByActiveItem = 1 << 7,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
        AllowWhenOverlappedByItem = 1 << 8,   // IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
        AllowWhenOverlappedByWindow = 1 << 9,   // IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
        AllowWhenDisabled = 1 << 10,  // IsItemHovered() only: Return true even if the item is disabled
        NoNavOverride = 1 << 11,  // IsItemHovered() only: Disable using gamepad/keyboard navigation state when active, always query mouse
        AllowWhenOverlapped = AllowWhenOverlappedByItem | AllowWhenOverlappedByWindow,
        RectOnly = AllowWhenBlockedByPopup | AllowWhenBlockedByActiveItem | AllowWhenOverlapped,
        RootAndChildWindows = RootWindow | ChildWindows,

        // Tooltips mode
        // - typically used in IsItemHovered() + SetTooltip() sequence.
        // - this is a shortcut to pull flags from 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' where you can reconfigure desired behavior.
        //   e.g. 'TooltipHoveredFlagsForMouse' defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort'.
        // - for frequently actioned or hovered items providing a tooltip, you want may to use ImGuiHoveredFlags_ForTooltip (stationary + delay) so the tooltip doesn't show too often.
        // - for items which main purpose is to be hovered, or items with low affordance, or in less consistent apps, prefer no delay or shorter delay.
        ForTooltip = 1 << 12,  // Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.

        // (Advanced) Mouse Hovering delays.
        // - generally you can use ImGuiHoveredFlags_ForTooltip to use application-standardized flags.
        // - use those if you need specific overrides.
        Stationary = 1 << 13,  // Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
        DelayNone = 1 << 14,  // IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
        DelayShort = 1 << 15,  // IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
        DelayNormal = 1 << 16,  // IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
        NoSharedDelay = 1 << 17,  // IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiHoveredFlags);

    enum class GuiPopupFlags
    {
        None = 0,
        MouseButtonLeft = 0,        // For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as ImGuiMouseButton_Left)
        MouseButtonRight = 1,        // For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as ImGuiMouseButton_Right)
        MouseButtonMiddle = 2,        // For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as ImGuiMouseButton_Middle)
        MouseButtonMask_ = 0x1F,
        MouseButtonDefault_ = 1,
        NoOpenOverExistingPopup = 1 << 5,   // For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
        NoOpenOverItems = 1 << 6,   // For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
        AnyPopupId = 1 << 7,   // For IsPopupOpen(): ignore the ImGuiID parameter and test for any popup.
        AnyPopupLevel = 1 << 8,   // For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
        AnyPopup = AnyPopupId | AnyPopupLevel,
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiPopupFlags);

    enum class GuiSelectableFlags
    {
        None = 0,
        DontClosePopups = 1 << 0,   // Clicking this doesn't close parent popup window
        SpanAllColumns = 1 << 1,   // Selectable frame can span all columns (text will still fit in current column)
        AllowDoubleClick = 1 << 2,   // Generate press events on double clicks too
        Disabled = 1 << 3,   // Cannot be selected, display grayed out text
        AllowOverlap = 1 << 4,   // (WIP) Hit testing to allow subsequent widgets to overlap this one
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiSelectableFlags);

    enum class GuiColorID
    {
        Text,
        TextDisabled,
        WindowBg,              // Background of normal windows
        ChildBg,               // Background of child windows
        PopupBg,               // Background of popups, menus, tooltips windows
        Border,
        BorderShadow,
        FrameBg,               // Background of checkbox, radio button, plot, slider, text input
        FrameBgHovered,
        FrameBgActive,
        TitleBg,
        TitleBgActive,
        TitleBgCollapsed,
        MenuBarBg,
        ScrollbarBg,
        ScrollbarGrab,
        ScrollbarGrabHovered,
        ScrollbarGrabActive,
        CheckMark,
        SliderGrab,
        SliderGrabActive,
        Button,
        ButtonHovered,
        ButtonActive,
        Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
        HeaderHovered,
        HeaderActive,
        Separator,
        SeparatorHovered,
        SeparatorActive,
        ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
        ResizeGripHovered,
        ResizeGripActive,
        Tab,                   // TabItem in a TabBar
        TabHovered,
        TabActive,
        TabUnfocused,
        TabUnfocusedActive,
        DockingPreview,        // Preview overlay color when about to docking something
        DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
        PlotLines,
        PlotLinesHovered,
        PlotHistogram,
        PlotHistogramHovered,
        TableHeaderBg,         // Table header background
        TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
        TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
        TableRowBg,            // Table row background (even rows)
        TableRowBgAlt,         // Table row background (odd rows)
        TextSelectedBg,
        DragDropTarget,        // Rectangle highlighting a drop target
        NavHighlight,          // Gamepad/keyboard: current highlighted item
        NavWindowingHighlight, // Highlight window when using CTRL+TAB
        NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
        COUNT
    };

    constexpr Size GUI_COLOR_ID_COUNT = static_cast<Size>(GuiColorID::COUNT);

    enum class GuiStyleID
    {
        // Enum name --------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
        Alpha,               // float     Alpha
        DisabledAlpha,       // float     DisabledAlpha
        WindowPadding,       // ImVec2    WindowPadding
        WindowRounding,      // float     WindowRounding
        WindowBorderSize,    // float     WindowBorderSize
        WindowMinSize,       // ImVec2    WindowMinSize
        WindowTitleAlign,    // ImVec2    WindowTitleAlign
        ChildRounding,       // float     ChildRounding
        ChildBorderSize,     // float     ChildBorderSize
        PopupRounding,       // float     PopupRounding
        PopupBorderSize,     // float     PopupBorderSize
        FramePadding,        // ImVec2    FramePadding
        FrameRounding,       // float     FrameRounding
        FrameBorderSize,     // float     FrameBorderSize
        ItemSpacing,         // ImVec2    ItemSpacing
        ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
        IndentSpacing,       // float     IndentSpacing
        CellPadding,         // ImVec2    CellPadding
        ScrollbarSize,       // float     ScrollbarSize
        ScrollbarRounding,   // float     ScrollbarRounding
        GrabMinSize,         // float     GrabMinSize
        GrabRounding,        // float     GrabRounding
        TabRounding,         // float     TabRounding
        TabBarBorderSize,    // float     TabBarBorderSize
        ButtonTextAlign,     // ImVec2    ButtonTextAlign
        SelectableTextAlign, // ImVec2    SelectableTextAlign
        SeparatorTextBorderSize,// float  SeparatorTextBorderSize
        SeparatorTextAlign,  // ImVec2    SeparatorTextAlign
        SeparatorTextPadding,// ImVec2    SeparatorTextPadding
        DockingSeparatorSize,// float     DockingSeparatorSize
        COUNT
    };

    constexpr Size GUI_STYLE_ID_COUNT = static_cast<Size>(GuiStyleID::COUNT);

    enum class GuiDockFlags
    {
        None = 0,
        KeepAliveOnly = 1 << 0,   //       // Don't display the dockspace node but keep it alive. Windows docked into this dockspace node won't be undocked.
        //NoCentralNode              = 1 << 1,   //       // Disable Central Node (the node which can stay empty)
        NoDockingOverCentralNode = 1 << 2,   //       // Disable docking over the Central Node, which will be always kept empty.
        PassthruCentralNode = 1 << 3,   //       // Enable passthru dockspace: 1) DockSpace() will render a ImGuiCol_WindowBg background covering everything excepted the Central Node when empty. Meaning the host window should probably use SetNextWindowBgAlpha(0.0f) prior to Begin() when using this. 2) When Central Node is empty: let inputs pass-through + won't display a DockingEmptyBg background. See demo for details.
        NoDockingSplit = 1 << 4,   //       // Disable other windows/nodes from splitting this node.
        NoResize = 1 << 5,   // Saved // Disable resizing node using the splitter/separators. Useful with programmatically setup dockspaces.
        AutoHideTabBar = 1 << 6,   //       // Tab bar will automatically hide when there is a single window in the dock node.
        NoUndocking = 1 << 7,   //       // Disable undocking this node.
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiDockFlags);

    enum class GuiConfigFlags
    {
        ImGuiConfigFlags_None = 0,
        ImGuiConfigFlags_NavEnableKeyboard = 1 << 0,   // Master keyboard navigation enable flag. Enable full Tabbing + directional arrows + space/enter to activate.
        ImGuiConfigFlags_NavEnableGamepad = 1 << 1,   // Master gamepad navigation enable flag. Backend also needs to set ImGuiBackendFlags_HasGamepad.
        ImGuiConfigFlags_NavEnableSetMousePos = 1 << 2,   // Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=true. If enabled you MUST honor io.WantSetMousePos requests in your backend, otherwise ImGui will react as if the mouse is jumping around back and forth.
        ImGuiConfigFlags_NavNoCaptureKeyboard = 1 << 3,   // Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.
        ImGuiConfigFlags_NoMouse = 1 << 4,   // Instruct imgui to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the backend.
        ImGuiConfigFlags_NoMouseCursorChange = 1 << 5,   // Instruct backend to not alter mouse cursor shape and visibility. Use if the backend cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from imgui by reading GetMouseCursor() yourself instead.

        // [BETA] Docking
        ImGuiConfigFlags_DockingEnable = 1 << 6,   // Docking enable flags.

        // [BETA] Viewports
        // When using viewports it is recommended that your default value for ImGuiCol_WindowBg is opaque (Alpha=1.0) so transition to a viewport won't be noticeable.
        ImGuiConfigFlags_ViewportsEnable = 1 << 10,  // Viewport enable flags (require both ImGuiBackendFlags_PlatformHasViewports + ImGuiBackendFlags_RendererHasViewports set by the respective backends)
        ImGuiConfigFlags_DpiEnableScaleViewports = 1 << 14,  // [BETA: Don't use] FIXME-DPI: Reposition and resize imgui windows when the DpiScale of a viewport changed (mostly useful for the main viewport hosting other window). Note that resizing the main window itself is up to your application.
        ImGuiConfigFlags_DpiEnableScaleFonts = 1 << 15,  // [BETA: Don't use] FIXME-DPI: Request bitmap-scaled fonts to match DpiScale. This is a very low-quality workaround. The correct way to handle DPI is _currently_ to replace the atlas and/or fonts in the Platform_OnChangedViewport callback, but this is all early work in progress.

        // User storage (to allow your backend/engine to communicate to code that may be shared between multiple projects. Those flags are NOT used by core Dear ImGui)
        ImGuiConfigFlags_IsSRGB = 1 << 20,  // Application is SRGB-aware.
        ImGuiConfigFlags_IsTouchScreen = 1 << 21,  // Application is using a touch screen instead of a mouse.
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiConfigFlags);

    enum class GuiViewportFlags
    {
        None = 0,
        IsPlatformWindow = 1 << 0,   // Represent a Platform Window
        IsPlatformMonitor = 1 << 1,   // Represent a Platform Monitor (unused yet)
        OwnedByApp = 1 << 2,   // Platform Window: Was created/managed by the user application? (rather than our backend)
        NoDecoration = 1 << 3,   // Platform Window: Disable platform decorations: title bar, borders, etc. (generally set all windows, but if ImGuiConfigFlags_ViewportsDecoration is set we only set this on popups/tooltips)
        NoTaskBarIcon = 1 << 4,   // Platform Window: Disable platform task bar icon (generally set on popups/tooltips, or all windows if ImGuiConfigFlags_ViewportsNoTaskBarIcon is set)
        NoFocusOnAppearing = 1 << 5,   // Platform Window: Don't take focus when created.
        NoFocusOnClick = 1 << 6,   // Platform Window: Don't take focus when clicked on.
        NoInputs = 1 << 7,   // Platform Window: Make mouse pass through so we can drag this window while peaking behind it.
        NoRendererClear = 1 << 8,   // Platform Window: Renderer doesn't need to clear the framebuffer ahead (because we will fill it entirely).
        NoAutoMerge = 1 << 9,   // Platform Window: Avoid merging this window into another host window. This can only be set via ImGuiWindowClass viewport flags override (because we need to now ahead if we are going to create a viewport in the first place!).
        TopMost = 1 << 10,  // Platform Window: Display on top (for tooltips only).
        CanHostOtherWindows = 1 << 11,  // Viewport can host multiple imgui windows (secondary viewports are associated to a single window). // FIXME: In practice there's still probably code making the assumption that this is always and only on the MainViewport. Will fix once we add support for "no main viewport".

        // Output status flags (from Platform)
        IsMinimized = 1 << 12,  // Platform Window: Window is minimized, can skip render. When minimized we tend to avoid using the viewport pos/size for clipping window or testing if they are contained in the viewport.
        IsFocused = 1 << 13,  // Platform Window: Window is focused (last call to Platform_GetWindowFocus() returned true)
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiViewportFlags);

    enum class GuiBackendFlags
    {
        None = 0,
        HasGamepad = 1 << 0,   // Backend Platform supports gamepad and currently has one connected.
        HasMouseCursors = 1 << 1,   // Backend Platform supports honoring GetMouseCursor() value to change the OS cursor shape.
        HasSetMousePos = 1 << 2,   // Backend Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if ImGuiConfigFlags_NavEnableSetMousePos is set).
        RendererHasVtxOffset = 1 << 3,   // Backend Renderer supports ImDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.

        // [BETA] Viewports
        PlatformHasViewports = 1 << 10,  // Backend Platform supports multiple viewports.
        HasMouseHoveredViewport = 1 << 11,  // Backend Platform supports calling io.AddMouseViewportEvent() with the viewport under the mouse. IF POSSIBLE, ignore viewports with the ImGuiViewportFlags_NoInputs flag (Win32 backend, GLFW 3.30+ backend can do this, SDL backend cannot). If this cannot be done, Dear ImGui needs to use a flawed heuristic to find the viewport under.
        RendererHasViewports = 1 << 12,  // Backend Renderer supports multiple viewports.
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiBackendFlags);

    enum class GuiFocusedFlags
    {
        None = 0,
        ChildWindows = 1 << 0,   // Return true if any children of the window is focused
        RootWindow = 1 << 1,   // Test from root window (top most parent of the current hierarchy)
        AnyWindow = 1 << 2,   // Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
        NoPopupHierarchy = 1 << 3,   // Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
        DockHierarchy = 1 << 4,   // Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
        RootAndChildWindows = RootWindow | ChildWindows,
    };

    MINTY_ENUM_FLAGS_OPERATORS(GuiFocusedFlags);
}