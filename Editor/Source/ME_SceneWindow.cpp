#include "ME_SceneWindow.h"

#include "ME_EditorApplication.h"

using namespace Minty;
using namespace Mintye;

constexpr static float MOUSE_MOVE_SENSITIVITY = 0.02f;
constexpr static float MOUSE_ROTATE_SENSITIVITY = 0.1f;

Mintye::SceneWindow::SceneWindow(EditorApplication& application)
	: EditorWindow(application)
	, _cameraPosition()
	, _cameraRotation()
	, _camera()
	, _isMouseMoving()
	, _isMouseRotating()
	, _mouseDragPos()
{}

void Mintye::SceneWindow::draw()
{
	GUI::set_next_window_size(Float2(400, 400), GuiCondition::FirstUseEver);
	if (!GUI::begin("Scene", nullptr, GuiWindowFlags::NoBackground))
	{
		GUI::end();
		return;
	}

	Renderer::set_camera(_cameraPosition, _cameraRotation, _camera);

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
		if (!_isMouseMoving && !_isMouseRotating && GUI::is_mouse_down(MouseButton::Middle))
		{
			// first down
			_isMouseMoving = true;
			_mouseDragPos = GUI::get_mouse_position();
		}

		if (!_isMouseMoving && !_isMouseRotating && GUI::is_mouse_down(MouseButton::Right))
		{
			_isMouseRotating = true;
			_mouseDragPos = GUI::get_mouse_position();
		}

		if (_isMouseMoving)
		{
			// move
			Float2 mousePos = GUI::get_mouse_position();

			_cameraPosition += -1.0f * MOUSE_MOVE_SENSITIVITY * (right(_cameraRotation) * (mousePos.x - _mouseDragPos.x));
			_cameraPosition += 1.0f * MOUSE_MOVE_SENSITIVITY * (up(_cameraRotation) * (mousePos.y - _mouseDragPos.y));

			_mouseDragPos = mousePos;
		}

		if (_isMouseRotating)
		{
			// yaw is Y, pitch is X

			Float2 mousePos = GUI::get_mouse_position();

			_cameraOrientation += MOUSE_ROTATE_SENSITIVITY * Float3(-(mousePos.y - _mouseDragPos.y), (mousePos.x - _mouseDragPos.x), 0.0f);
			_cameraOrientation.x = Math::clamp(_cameraOrientation.x, -90.0f + 0.1f, 90.0f - 0.1f);

			_mouseDragPos = mousePos;

			// set yaw and pitch back
			_cameraRotation = from_euler(_cameraOrientation);
		}

		_cameraPosition += forward(_cameraRotation) * GUI::get_mouse_wheel();
	}

	if (_isMouseMoving && !GUI::is_mouse_down(MouseButton::Middle))
	{
		// stop moving
		_isMouseMoving = false;
	}

	if (_isMouseRotating && !GUI::is_mouse_down(MouseButton::Right))
	{
		// stop rotating
		_isMouseRotating = false;
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
		_cameraPosition = transformComponent->get_global_position() - forward(_cameraRotation) * transformComponent->get_global_scale().z;
	}
}
