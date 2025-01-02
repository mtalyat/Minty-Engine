#include "ME_SceneWindow.h"

#include "ME_EditorApplication.h"

using namespace Minty;
using namespace Mintye;

constexpr static float MOUSE_MOVE_SENSITIVITY = 0.02f;
constexpr static float MOUSE_ROTATE_SENSITIVITY = 0.1f;

Mintye::SceneWindow::SceneWindow(EditorApplication& application)
	: EditorWindow(application)
	, m_cameraPosition()
	, m_cameraOrientation()
	, m_cameraRotation()
	, m_camera()
	, m_isMouseMoving()
	, m_isMouseRotating()
	, m_mouseDragPos()
{}

void Mintye::SceneWindow::draw()
{
	GUI::set_next_window_size(Float2(400, 400), GuiCondition::FirstUseEver);
	if (!GUI::begin("Scene", nullptr, GuiWindowFlags::NoBackground))
	{
		GUI::end();
		return;
	}

	Renderer::set_camera(m_cameraPosition, m_cameraRotation, m_camera);

	// for now, move the render location to fake a scene window
	Float2 windowPos = GUI::get_window_position();

	Float2 min = Float2(0.0f, 0.0f); //ImGui::GetWindowContentRegionMin();
	Float2 max = GUI::get_window_size(); //ImGui::GetWindowContentRegionMax();

	Float2 pos = Float2(min.x + windowPos.x, min.y + windowPos.y);
	Float2 size = Float2(max.x - min.x, max.y - min.y);

	Ref<Viewport> viewport = Renderer::get_viewport();
	viewport->set_x(pos.x);
	viewport->set_y(pos.y);
	viewport->set_width(size.x);
	viewport->set_height(size.y);

	// get input
	if (GUI::is_window_hovered())
	{
		if (!m_isMouseMoving && !m_isMouseRotating && GUI::is_mouse_down(MouseButton::Middle))
		{
			// first down
			m_isMouseMoving = true;
			m_mouseDragPos = GUI::get_mouse_position();
		}

		if (!m_isMouseMoving && !m_isMouseRotating && GUI::is_mouse_down(MouseButton::Right))
		{
			m_isMouseRotating = true;
			m_mouseDragPos = GUI::get_mouse_position();
		}

		if (m_isMouseMoving)
		{
			// move
			Float2 mousePos = GUI::get_mouse_position();

			m_cameraPosition += -1.0f * MOUSE_MOVE_SENSITIVITY * (right(m_cameraRotation) * (mousePos.x - m_mouseDragPos.x));
			m_cameraPosition += 1.0f * MOUSE_MOVE_SENSITIVITY * (up(m_cameraRotation) * (mousePos.y - m_mouseDragPos.y));

			m_mouseDragPos = mousePos;
		}

		if (m_isMouseRotating)
		{
			// yaw is Y, pitch is X

			Float2 mousePos = GUI::get_mouse_position();

			m_cameraOrientation += MOUSE_ROTATE_SENSITIVITY * Float3(-(mousePos.y - m_mouseDragPos.y), (mousePos.x - m_mouseDragPos.x), 0.0f);
			m_cameraOrientation.x = Math::clamp(m_cameraOrientation.x, -90.0f + 0.1f, 90.0f - 0.1f);

			m_mouseDragPos = mousePos;

			// set yaw and pitch back
			m_cameraRotation = from_euler(m_cameraOrientation);
		}

		m_cameraPosition += forward(m_cameraRotation) * GUI::get_mouse_wheel();
	}

	if (m_isMouseMoving && !GUI::is_mouse_down(MouseButton::Middle))
	{
		// stop moving
		m_isMouseMoving = false;
	}

	if (m_isMouseRotating && !GUI::is_mouse_down(MouseButton::Right))
	{
		// stop rotating
		m_isMouseRotating = false;
	}

	GUI::end();
}

void Mintye::SceneWindow::reset()
{
}

void Mintye::SceneWindow::refresh()
{
	
}

void Mintye::SceneWindow::focus(Minty::Entity const entity)
{
	// move camera to entity - camera forward
	EntityRegistry& registry = get_scene()->get_entity_registry();

	if (TransformComponent* transformComponent = registry.try_get<TransformComponent>(entity))
	{
		m_cameraPosition = transformComponent->get_global_position() - forward(m_cameraRotation) * transformComponent->get_global_scale().z;
	}
}
