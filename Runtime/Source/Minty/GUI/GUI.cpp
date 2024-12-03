#include "pch.h"
#include "GUI.h"

#include "Minty/Core/Macros.h"
#include "Minty/Core/Operation.h"
#include "Minty/Library/ImGUI.h"
#include "Minty/Window/WindowManager.h"
#if defined(MINTY_VULKAN)
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#endif
#include <unordered_map>

using namespace Minty;

#if defined(MINTY_VULKAN)
static VkDescriptorPool s_imGuiDescriptorPool;
#endif

Owner<RenderPass> GUI::s_renderPass = nullptr;
Ref<RenderTarget> GUI::s_renderTarget = nullptr;

#pragma region Helper

inline static ImVec2 float2_to_imvec2(Float2 const& value) { return ImVec2(value.x, value.y); }

inline static Float2 imvec2_to_float2(ImVec2 const& value) { return Float2(value.x, value.y); }

static ImGuiKey key_to_imguikey(Key const key)
{
	static const std::unordered_map<Key, ImGuiKey> keyToImGuiKeyMap = {
		{Key::Space, ImGuiKey_Space},
		{Key::Apostrophe, ImGuiKey_Apostrophe},
		{Key::Comma, ImGuiKey_Comma},
		{Key::Minus, ImGuiKey_Minus},
		{Key::Period, ImGuiKey_Period},
		{Key::Slash, ImGuiKey_Slash},
		{Key::D0, ImGuiKey_0},
		{Key::D1, ImGuiKey_1},
		{Key::D2, ImGuiKey_2},
		{Key::D3, ImGuiKey_3},
		{Key::D4, ImGuiKey_4},
		{Key::D5, ImGuiKey_5},
		{Key::D6, ImGuiKey_6},
		{Key::D7, ImGuiKey_7},
		{Key::D8, ImGuiKey_8},
		{Key::D9, ImGuiKey_9},
		{Key::Semicolon, ImGuiKey_Semicolon},
		{Key::Equal, ImGuiKey_Equal},
		{Key::A, ImGuiKey_A},
		{Key::B, ImGuiKey_B},
		{Key::C, ImGuiKey_C},
		{Key::D, ImGuiKey_D},
		{Key::E, ImGuiKey_E},
		{Key::F, ImGuiKey_F},
		{Key::G, ImGuiKey_G},
		{Key::H, ImGuiKey_H},
		{Key::I, ImGuiKey_I},
		{Key::J, ImGuiKey_J},
		{Key::K, ImGuiKey_K},
		{Key::L, ImGuiKey_L},
		{Key::M, ImGuiKey_M},
		{Key::N, ImGuiKey_N},
		{Key::O, ImGuiKey_O},
		{Key::P, ImGuiKey_P},
		{Key::Q, ImGuiKey_Q},
		{Key::R, ImGuiKey_R},
		{Key::S, ImGuiKey_S},
		{Key::T, ImGuiKey_T},
		{Key::U, ImGuiKey_U},
		{Key::V, ImGuiKey_V},
		{Key::W, ImGuiKey_W},
		{Key::X, ImGuiKey_X},
		{Key::Y, ImGuiKey_Y},
		{Key::Z, ImGuiKey_Z},
		{Key::LeftBracket, ImGuiKey_LeftBracket},
		{Key::Backslash, ImGuiKey_Backslash},
		{Key::RightBracket, ImGuiKey_RightBracket},
		{Key::Grave, ImGuiKey_GraveAccent},
		{Key::Escape, ImGuiKey_Escape},
		{Key::Enter, ImGuiKey_Enter},
		{Key::Tab, ImGuiKey_Tab},
		{Key::Backspace, ImGuiKey_Backspace},
		{Key::Insert, ImGuiKey_Insert},
		{Key::Delete, ImGuiKey_Delete},
		{Key::Right, ImGuiKey_RightArrow},
		{Key::Left, ImGuiKey_LeftArrow},
		{Key::Down, ImGuiKey_DownArrow},
		{Key::Up, ImGuiKey_UpArrow},
		{Key::PageUp, ImGuiKey_PageUp},
		{Key::PageDown, ImGuiKey_PageDown},
		{Key::Home, ImGuiKey_Home},
		{Key::End, ImGuiKey_End},
		{Key::CapsLock, ImGuiKey_CapsLock},
		{Key::ScrollLock, ImGuiKey_ScrollLock},
		{Key::NumLock, ImGuiKey_NumLock},
		{Key::PrintScreen, ImGuiKey_PrintScreen},
		{Key::Pause, ImGuiKey_Pause},
		{Key::F1, ImGuiKey_F1},
		{Key::F2, ImGuiKey_F2},
		{Key::F3, ImGuiKey_F3},
		{Key::F4, ImGuiKey_F4},
		{Key::F5, ImGuiKey_F5},
		{Key::F6, ImGuiKey_F6},
		{Key::F7, ImGuiKey_F7},
		{Key::F8, ImGuiKey_F8},
		{Key::F9, ImGuiKey_F9},
		{Key::F10, ImGuiKey_F10},
		{Key::F11, ImGuiKey_F11},
		{Key::F12, ImGuiKey_F12},
		{Key::KeyPad0, ImGuiKey_Keypad0},
		{Key::KeyPad1, ImGuiKey_Keypad1},
		{Key::KeyPad2, ImGuiKey_Keypad2},
		{Key::KeyPad3, ImGuiKey_Keypad3},
		{Key::KeyPad4, ImGuiKey_Keypad4},
		{Key::KeyPad5, ImGuiKey_Keypad5},
		{Key::KeyPad6, ImGuiKey_Keypad6},
		{Key::KeyPad7, ImGuiKey_Keypad7},
		{Key::KeyPad8, ImGuiKey_Keypad8},
		{Key::KeyPad9, ImGuiKey_Keypad9},
		{Key::KeyPadDecimal, ImGuiKey_KeypadDecimal},
		{Key::KeyPadDivide, ImGuiKey_KeypadDivide},
		{Key::KeyPadMultiply, ImGuiKey_KeypadMultiply},
		{Key::KeyPadSubtract, ImGuiKey_KeypadSubtract},
		{Key::KeyPadAdd, ImGuiKey_KeypadAdd},
		{Key::KeyPadEnter, ImGuiKey_KeypadEnter},
		{Key::KeyPadEqual, ImGuiKey_KeypadEqual},
		{Key::LeftShift, ImGuiKey_LeftShift},
		{Key::LeftControl, ImGuiKey_LeftCtrl},
		{Key::LeftAlt, ImGuiKey_LeftAlt},
		{Key::LeftSuper, ImGuiKey_LeftSuper},
		{Key::RightShift, ImGuiKey_RightShift},
		{Key::RightControl, ImGuiKey_RightCtrl},
		{Key::RightAlt, ImGuiKey_RightAlt},
		{Key::RightSuper, ImGuiKey_RightSuper},
		{Key::Menu, ImGuiKey_Menu}
	};

	auto it = keyToImGuiKeyMap.find(key);
	return (it != keyToImGuiKeyMap.end()) ? it->second : ImGuiKey_None;
}

static ImGuiMouseButton mouse_button_to_imguimousebutton(MouseButton const button)
{
	return static_cast<ImGuiMouseButton>(button);
}

inline static ImVec4 color_to_imvec4(Color const color) { return ImVec4(color.rf(), color.gf(), color.bf(), color.af()); }

#pragma endregion

static GuiViewport s_mainViewport{};

void Minty::GUI::initialize(GUIBuilder const& builder)
{
	// create resources
	RenderPassBuilder renderPassBuilder{};
	RenderAttachment colorAttachment{};
	colorAttachment.type = RenderAttachmentType::Color;
	colorAttachment.format = Renderer::get_color_format();
	colorAttachment.loadOperation = RenderAttachmentLoadOperation::Load;
	colorAttachment.storeOperation = RenderAttachmentStoreOperation::Store;
	colorAttachment.initialLayout = ImageLayout::ColorAttachmentOptimal;
	colorAttachment.finalLayout = ImageLayout::PresentSrcKhr;
	renderPassBuilder.colorAttachment = &colorAttachment;
	s_renderPass = Renderer::create_render_pass(renderPassBuilder);
	s_renderTarget = Renderer::create_render_target(s_renderPass);

	// create context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// TODO: add the following to the builder

	// change some config
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// set up style
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// init based on types
	Ref<Window> mainWindow = WindowManager::get_main();
#if defined(MINTY_WINDOWS) && defined(MINTY_VULKAN)
	GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(mainWindow->get_native());
	ImGui_ImplGlfw_InitForVulkan(nativeWindow, true);

	// create a descriptor pool for ImGui
	std::array<VkDescriptorPoolSize, 1> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(VulkanRenderer::get_device(), &poolInfo, nullptr, &s_imGuiDescriptorPool) != VK_SUCCESS)\
	{
		MINTY_ABORT("Failed to create GUI descriptor pool.");
	}

	// init vulkan with ImGui
	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = VulkanRenderer::get_instance();
	initInfo.PhysicalDevice = VulkanRenderer::get_physical_device();
	initInfo.Device = VulkanRenderer::get_device();
	initInfo.QueueFamily = VulkanRenderer::get_queue_family_indices().graphicsFamily.value();
	initInfo.Queue = VulkanRenderer::get_graphics_queue();
	initInfo.PipelineCache = VK_NULL_HANDLE;
	initInfo.DescriptorPool = s_imGuiDescriptorPool;
	initInfo.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	initInfo.ImageCount = MAX_FRAMES_IN_FLIGHT;
	initInfo.Allocator = nullptr;
	initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	Ref<VulkanRenderPass> vulkanRenderPass = static_cast<Ref<VulkanRenderPass>>(s_renderPass);
	ImGui_ImplVulkan_Init(&initInfo, vulkanRenderPass->get_render_pass());

	// upload ImGui fonts
	VkCommandBuffer commandBuffer = VulkanRenderer::begin_command_buffer_single();
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	VulkanRenderer::end_command_buffer_single(commandBuffer, VulkanRenderer::get_graphics_queue());
	VulkanRenderer::sync();
	ImGui_ImplVulkan_DestroyFontUploadObjects();
#else
	MINTY_ABORT("Unable to initialize GUI. Currently, only Vulkan and Windows are supported.");
#endif
}

void Minty::GUI::shutdown()
{
	// release resources
	s_renderPass.release();
	s_renderTarget.release();

	// shut down ImGui
#if defined(MINTY_VULKAN)
	ImGui_ImplVulkan_Shutdown();
#endif // MINTY_VULKAN
#if defined(MINTY_WINDOWS)
	ImGui_ImplGlfw_Shutdown();
#endif // MINTY_WINDOWS

	ImGui::DestroyContext();

	// shut down rendering resources
#if defined(MINTY_VULKAN)
	vkDestroyDescriptorPool(VulkanRenderer::get_device(), s_imGuiDescriptorPool, nullptr);
#endif // MINTY_VULKAN

	s_renderPass.release();
}

Int Minty::GUI::start_frame()
{
#if defined(MINTY_VULKAN)
	ImGui_ImplVulkan_NewFrame();
#else
	MINTY_ERROR("Unknown GUI platform.");
	return 1;
#endif // MINTY_VULKAN
#if defined(MINTY_WINDOWS)
	ImGui_ImplGlfw_NewFrame();
#else
	MINTY_ERROR("Unknown GUI OS.");
	return 2;
#endif // MINTY_WINDOWS
	ImGui::NewFrame();

	return 0;
}

void Minty::GUI::end_frame()
{
	ImGui::Render();
#if defined(MINTY_VULKAN)
	Ref<VulkanRenderPass> vulkanRenderPass = static_cast<Ref<VulkanRenderPass>>(s_renderPass);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VulkanRenderer::get_command_buffer());
#else
	MINTY_ERROR("Unknown GUI platform.");
	return;
#endif // MINTY_VULKAN
#if defined(MINTY_WINDOWS)

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
#else
	MINTY_ERROR("Unknown GUI OS.");
	return false;
#endif // MINTY_WINDOWS
}

void Minty::GUI::start_pass()
{
}

void Minty::GUI::end_pass()
{
}

Bool Minty::GUI::begin(String const& name, Bool* const open, GuiWindowFlags const flags)
{
	return ImGui::Begin(name.c_str(), open, static_cast<ImGuiWindowFlags>(flags));
}

void Minty::GUI::end()
{
	ImGui::End();
}

Bool Minty::GUI::begin_child(String const& id, Float2 const& size, Bool const border, GuiWindowFlags const flags)
{
	return ImGui::BeginChild(id.c_str(), float2_to_imvec2(size), border, static_cast<ImGuiWindowFlags>(flags));
}

void Minty::GUI::end_child()
{
	ImGui::EndChild();
}

Float2 Minty::GUI::get_cursor_position()
{
	return imvec2_to_float2(ImGui::GetCursorPos());
}

Float Minty::GUI::get_cursor_position_x()
{
	return ImGui::GetCursorPosX();
}

Float Minty::GUI::get_cursor_position_y()
{
	return ImGui::GetCursorPosY();
}

void Minty::GUI::set_cursor_position(Float2 const& pos)
{
	ImGui::SetCursorPos(float2_to_imvec2(pos));
}

void Minty::GUI::set_cursor_position_x(Float const x)
{
	ImGui::SetCursorPosX(x);
}

void Minty::GUI::set_cursor_position_y(Float const y)
{
	ImGui::SetCursorPosY(y);
}

void Minty::GUI::begin_disabled(Bool const disabled)
{
	ImGui::BeginDisabled(disabled);
}

void Minty::GUI::end_disabled()
{
	ImGui::EndDisabled();
}

void Minty::GUI::dock_space(String const& id, Float2 const& size, GuiDockFlags const flags)
{
	ImGuiID imId = ImGui::GetID(id.c_str());
	ImGui::DockSpace(imId, float2_to_imvec2(size), static_cast<ImGuiDockNodeFlags>(flags));
}

Bool Minty::GUI::button(String const& label, Float2 const& size)
{
	return ImGui::Button(label.c_str(), float2_to_imvec2(size));
}

Bool Minty::GUI::selectable(String const& label, Bool const selected, GuiSelectableFlags const flags, Float2 const& size)
{
	return ImGui::Selectable(label.c_str(), selected, static_cast<ImGuiSelectableFlags>(flags), float2_to_imvec2(size));
}

Bool Minty::GUI::selectable(String const& label, Bool& selected, GuiSelectableFlags const flags, Float2 const& size)
{
	return ImGui::Selectable(label.c_str(), &selected, static_cast<ImGuiSelectableFlags>(flags), float2_to_imvec2(size));
}

Bool Minty::GUI::checkbox(String const& label, Bool& value)
{
	return ImGui::Checkbox(label.c_str(), &value);
}

void Minty::GUI::text(String const& text)
{
	ImGui::Text(text.c_str());
}

void Minty::GUI::text_wrapped(String const& text)
{
	ImGui::TextWrapped(text.c_str());
}

Bool Minty::GUI::input_text(String const& label, Char* const buffer, Size const bufferSize, GuiInputTextFlags const flags)
{
	return ImGui::InputText(label.c_str(), buffer, bufferSize, static_cast<ImGuiInputTextFlags>(flags));
}

void Minty::GUI::node(Node const& rootNode, Bool const showRootNode, Bool const recursive)
{
	std::vector<std::pair<Node const*, int>> nodes;
	nodes.push_back({ &rootNode, showRootNode ? 0 : -1 });

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node const*, int> pair = nodes.back();
		nodes.pop_back();
		Node const* node = pair.first;

		// print, if indent is >= 0
		if (pair.second >= 0)
		{
			String string = to_string(*node);
			ImGui::Text(String(static_cast<size_t>(pair.second << 1), ' ').append(string.empty() ? "_" : string).c_str());
		}

		// add children, in reverse so first children get dealth with first
		std::vector<Node> const& children = node->get_children();
		for (auto it = children.crbegin(); it != children.crend(); it++)
		{
			nodes.push_back({ &(*it), pair.second + 1 });
		}
	}
}

Bool Minty::GUI::input_node(Node& rootNode, Bool const showRootNode, Size const offset, Bool const recursive)
{
	static int const BUFFER_SIZE = 256;
	static char buffer[BUFFER_SIZE] = "";

	std::vector<std::pair<Node*, int>> nodes;
	nodes.push_back({ &rootNode, showRootNode ? 0 : -1 });

	bool modified = false;

	size_t i = offset;

	while (!nodes.empty())
	{
		// get next node
		std::pair<Node*, int> pair = nodes.back();
		nodes.pop_back();

		// ignore < 0 indents
		if (pair.second < 0) continue;

		String indentString = String(static_cast<size_t>(pair.second << 1), ' ');

		// print, if indent is >= 0
		if (pair.second == 0)
		{
			// display text on root
			String string = to_string(*pair.first);
			ImGui::Text(indentString.append(string.empty() ? "_" : string).c_str());
		}
		else
		{
			// input text

			size_t size = std::min(static_cast<size_t>(BUFFER_SIZE), pair.first->get_data().size() + 1);
			memcpy(buffer, pair.first->get_data_as_string().c_str(), size);
			buffer[size - 1] = '\0';

			ImGui::Text(std::format("{}{}: ", indentString, pair.first->get_name()).c_str());
			ImGui::SameLine();
			if (ImGui::InputText(std::format("##{}", i).c_str(), buffer, BUFFER_SIZE))
			{
				pair.first->set_data(buffer);
				modified = true;
			}
		}

		// add children, in reverse so first children get dealt with first
		std::vector<Node>& children = pair.first->get_children();
		for (auto it = children.rbegin(); it != children.rend(); it++)
		{
			nodes.push_back({ &(*it), pair.second + 1 });
		}

		i++;
	}

	return modified;
}

Bool Minty::GUI::input_text_expand(String const& label, Char* const buffer, Size const bufferSize, Float const widthPercent, GuiInputTextFlags const flags)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * widthPercent);
	return ImGui::InputText(label.c_str(), buffer, bufferSize, static_cast<ImGuiInputTextFlags>(flags));
}

Bool Minty::GUI::input_text_expand_offset(String const& label, Char* const buffer, Size const bufferSize, Float const offsetLeft, Float const offsetRight, Float const widthPercent, GuiInputTextFlags const flags)
{
	float avail = ImGui::GetContentRegionAvail().x;
	ImGui::SetNextItemWidth((avail - offsetLeft - offsetRight) * widthPercent + offsetLeft);
	return ImGui::InputText(label.c_str(), buffer, bufferSize, static_cast<ImGuiInputTextFlags>(flags));
}

Bool Minty::GUI::input_text_multiline_expand(String const& label, Char* const buffer, Size const bufferSize, Float const widthPercent, GuiInputTextFlags const flags)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * widthPercent);
	return ImGui::InputTextMultiline(label.c_str(), buffer, bufferSize, ImVec2(), static_cast<ImGuiInputTextFlags>(flags));
}

Bool Minty::GUI::input_text_multiline_expand_offset(String const& label, Char* const buffer, Size const bufferSize, Float const offsetLeft, Float const offsetRight, Float const widthPercent, GuiInputTextFlags const flags)
{
	float avail = ImGui::GetContentRegionAvail().x;
	ImGui::SetNextItemWidth((avail - offsetLeft - offsetRight) * widthPercent + offsetLeft);
	return ImGui::InputTextMultiline(label.c_str(), buffer, bufferSize, ImVec2(), static_cast<ImGuiInputTextFlags>(flags));
}

Bool Minty::GUI::button_left(String const& label, Float2 const& size)
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 imSize = float2_to_imvec2(size);

	String labelId = String("##").append(label).append("button");
	bool clicked = ImGui::Button(labelId.c_str(), imSize);

	ImVec2 endingPos = ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y);

	ImVec2 labelSize = ImGui::CalcTextSize(label.c_str());
	ImGui::SetCursorScreenPos(ImVec2(pos.x + 4.0f, pos.y + (imSize.y - labelSize.y) * 0.5f));
	ImGui::TextUnformatted(label.c_str());

	// make a dummy object for sizing purposes
	ImGui::SetCursorScreenPos(pos);
	ImGui::Dummy(imSize);

	return clicked;
}

void Minty::GUI::file_dialog_open(String const& key, String const& title, String const& filters, GuiFileDialogBuilder const& builder)
{
	IGFD::FileDialogConfig config{};
	config.path = builder.path.string();

	ImGuiFileDialog::Instance()->OpenDialog(key, title, filters.empty() ? nullptr : filters.c_str(), config);
}

Bool Minty::GUI::file_dialog_display(String const& key, GuiWindowFlags const flags, Float2 const& minSize, Float2 const& maxSize)
{
	return ImGuiFileDialog::Instance()->Display(key, static_cast<ImGuiWindowFlags>(flags), float2_to_imvec2(minSize), float2_to_imvec2(maxSize));
}

Bool Minty::GUI::file_dialog_is_ok()
{
	return ImGuiFileDialog::Instance()->IsOk();
}

Path Minty::GUI::file_dialog_get_path()
{
	return ImGuiFileDialog::Instance()->GetCurrentPath();
}

String Minty::GUI::file_dialog_get_file_name()
{
	return ImGuiFileDialog::Instance()->GetCurrentFileName();
}

void Minty::GUI::file_dialog_close()
{
	ImGuiFileDialog::Instance()->Close();
}

Float2 Minty::GUI::calculate_text_size(String const& text)
{
	return imvec2_to_float2(ImGui::CalcTextSize(text.c_str()));
}

void Minty::GUI::same_line(Float const offsetFromStartX, Float const spacing)
{
	ImGui::SameLine(offsetFromStartX, spacing);
}

void Minty::GUI::separator()
{
	ImGui::Separator();
}

void Minty::GUI::dummy(Float2 const& size)
{
	ImGui::Dummy(float2_to_imvec2(size));
}

void Minty::GUI::begin_group_box()
{
	ImGui::BeginGroup();
}

void Minty::GUI::end_group_box(Float2 const& size, Float2 const& margin, Color const borderColor)
{
	ImGui::EndGroup();

	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();

	// override size if size != 0
	if (size.x > 0.0f)
	{
		max.x = min.x + size.x;
	}
	if (size.y > 0.0f)
	{
		max.y = min.y + size.y;
	}

	// add margin
	min.x -= margin.x;
	max.x += margin.x;
	min.y -= margin.y;
	max.y += margin.y;

	ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(borderColor.r, borderColor.g, borderColor.b, borderColor.a));
}

Bool Minty::GUI::is_key_pressed(Key const key, Bool const repeat)
{
	return ImGui::IsKeyPressed(key_to_imguikey(key), repeat);
}

Bool Minty::GUI::is_key_modifier_pressed(KeyModifiers const keyModifier, Bool const repeat)
{
	// TODO: modify to support multiple modifiers at once
	ImGuiIO& io = ImGui::GetIO();
	switch (keyModifier)
	{
	case KeyModifiers::Shift:
		return io.KeyShift;
	case KeyModifiers::Control:
		return io.KeyCtrl;
	case KeyModifiers::Alt:
		return io.KeyAlt;
	case KeyModifiers::Super:
		return io.KeySuper;
	default:
		MINTY_NOT_IMPLEMENTED();
		return false;
	}
}

Bool Minty::GUI::is_mouse_down(MouseButton const button)
{
	return ImGui::IsMouseDown(mouse_button_to_imguimousebutton(button));
}

Bool Minty::GUI::is_mouse_released(MouseButton const button)
{
	return ImGui::IsMouseReleased(mouse_button_to_imguimousebutton(button));
}

Bool Minty::GUI::is_mouse_clicked(MouseButton const button, Bool const repeat)
{
	return ImGui::IsMouseClicked(mouse_button_to_imguimousebutton(button), repeat);
}

Bool Minty::GUI::is_mouse_double_clicked(MouseButton const button)
{
	return ImGui::IsMouseDoubleClicked(mouse_button_to_imguimousebutton(button));
}

void Minty::GUI::set_keyboard_focus_here(Int const offset)
{
	ImGui::SetKeyboardFocusHere(offset);
}

Handle Minty::GUI::get_id(String const& name)
{
	return static_cast<Handle>(ImGui::GetID(name.c_str()));
}

GuiConfigFlags Minty::GUI::get_config_flags()
{
	ImGuiIO& io = ImGui::GetIO();
	return static_cast<GuiConfigFlags>(io.ConfigFlags);
}

void Minty::GUI::set_config_flags(GuiConfigFlags const flags)
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = static_cast<ImGuiConfigFlags>(flags);
}

Float2 Minty::GUI::get_mouse_position()
{
	ImGuiIO& io = ImGui::GetIO();
	return imvec2_to_float2(io.MousePos);
}

Float Minty::GUI::get_mouse_wheel()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseWheel;
}

Float Minty::GUI::get_mouse_wheel_horizontal()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseWheelH;
}

Bool Minty::GUI::is_item_clicked(MouseButton const button)
{
	return ImGui::IsItemClicked(mouse_button_to_imguimousebutton(button));
}

Bool Minty::GUI::is_item_hovered(GuiHoveredFlags const flags)
{
	return ImGui::IsItemHovered(static_cast<ImGuiHoveredFlags>(flags));
}

Bool Minty::GUI::begin_main_menu_bar()
{
	return ImGui::BeginMainMenuBar();
}

void Minty::GUI::end_main_menu_bar()
{
	ImGui::EndMainMenuBar();
}

Bool Minty::GUI::begin_menu(String const& label, Bool const enabled)
{
	return ImGui::BeginMenu(label.c_str(), enabled);
}

void Minty::GUI::end_menu()
{
	ImGui::EndMenu();
}

Bool Minty::GUI::menu_item(String const& label, String const& shortcut, Bool const selected, Bool const enabled)
{
	return ImGui::MenuItem(label.c_str(), shortcut.c_str(), selected, enabled);
}

Bool Minty::GUI::menu_item(String const& label, String const& shortcut, Bool* const selected, Bool const enabled)
{
	return ImGui::MenuItem(label.c_str(), shortcut.empty() ? nullptr : shortcut.c_str(), selected, enabled);
}

Bool Minty::GUI::begin_popup(String const& id, GuiWindowFlags const flags)
{
	return ImGui::BeginPopup(id.c_str(), static_cast<ImGuiWindowFlags>(flags));
}

Bool Minty::GUI::begin_popup_modal(String const& name, Bool* const open, GuiWindowFlags const flags)
{
	return ImGui::BeginPopupModal(name.c_str(), open, static_cast<ImGuiWindowFlags>(flags));
}

Bool Minty::GUI::begin_popup_context_item(String const& id, GuiPopupFlags const flags)
{
	return ImGui::BeginPopupContextItem(id.c_str(), static_cast<ImGuiPopupFlags>(flags));
}

void Minty::GUI::end_popup()
{
	return ImGui::EndPopup();
}

void Minty::GUI::open_popup(String const& key, GuiPopupFlags const flags)
{
	return ImGui::OpenPopup(key.c_str(), static_cast<ImGuiPopupFlags>(flags));
}

void Minty::GUI::close_current_popup()
{
	ImGui::CloseCurrentPopup();
}

Float2 Minty::GUI::get_content_region_size()
{
	return imvec2_to_float2(ImGui::GetContentRegionAvail());
}

void Minty::GUI::push_style_color(GuiColorID const id, Color const color)
{
	ImGui::PushStyleColor(static_cast<ImGuiCol>(id), color_to_imvec4(color));
}

void Minty::GUI::pop_style_color(Int const count)
{
	ImGui::PopStyleColor(count);
}

void Minty::GUI::push_style_variable(GuiStyleID const id, Float const value)
{
	ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(id), value);
}

void Minty::GUI::push_style_variable(GuiStyleID const id, Float2 const& value)
{
	ImGui::PushStyleVar(static_cast<ImGuiStyleVar>(id), float2_to_imvec2(value));
}

void Minty::GUI::pop_style_variable(Int const count)
{
	ImGui::PopStyleVar(count);
}

Theme Minty::GUI::create_theme()
{
	// create theme
	Theme theme;

	// copy values from style
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 color;
	for (Size i = 0; i < GUI_COLOR_ID_COUNT; i++)
	{
		color = style.Colors[i];
		theme.at(i) = Color(color.x, color.y, color.z, color.w);
	}

	return theme;
}

void Minty::GUI::apply_theme(Theme const& theme)
{
	ImGuiStyle& style = ImGui::GetStyle();

	Color color;
	for (Size i = 0; i < GUI_COLOR_ID_COUNT; i++)
	{
		color = theme.at(i);
		style.Colors[i] = ImVec4(color.rf(), color.gf(), color.bf(), color.af());
	}
}

GuiViewport const& Minty::GUI::get_main_viewport()
{
	// update the static struct and return a reference to that
	ImGuiViewport const* const viewport = ImGui::GetMainViewport();
	s_mainViewport.id = viewport->ID;
	s_mainViewport.flags = static_cast<GuiViewportFlags>(viewport->Flags);
	s_mainViewport.position = imvec2_to_float2(viewport->Pos);
	s_mainViewport.size = imvec2_to_float2(viewport->Size);
	return s_mainViewport;
}

void Minty::GUI::set_next_window_position(Float2 const& pos, GuiCondition const condition, Float2 const& pivot)
{
	ImGui::SetNextWindowPos(float2_to_imvec2(pos), static_cast<ImGuiCond>(condition), float2_to_imvec2(pivot));
}

void Minty::GUI::set_next_window_size(Float2 const& size, GuiCondition const condition)
{
	ImGui::SetNextWindowSize(float2_to_imvec2(size), static_cast<ImGuiCond>(condition));
}

void Minty::GUI::set_next_window_viewport(Handle const id)
{
	ImGui::SetNextWindowViewport(static_cast<ImGuiID>(id));
}

Bool Minty::GUI::is_window_focused(GuiFocusedFlags const flags)
{
	return ImGui::IsWindowFocused(static_cast<ImGuiFocusedFlags>(flags));
}

Bool Minty::GUI::is_window_hovered(GuiHoveredFlags const flags)
{
	return ImGui::IsWindowHovered(static_cast<ImGuiHoveredFlags>(flags));
}

Float2 Minty::GUI::get_window_position()
{
	return imvec2_to_float2(ImGui::GetWindowPos());
}

Float2 Minty::GUI::get_window_size()
{
	return imvec2_to_float2(ImGui::GetWindowSize());
}
