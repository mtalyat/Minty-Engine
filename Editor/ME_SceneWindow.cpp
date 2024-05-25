#include "ME_SceneWindow.h"

#include "ME_EditorApplication.h"
#include "ME_EditorApplicationRenderEngine.h"

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
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoBackground))
	{
		ImGui::End();
		return;
	}

	EditorApplicationRenderEngine& renderer = EditorApplicationRenderEngine::instance();

	renderer.set_camera(_cameraPosition, _cameraRotation, _camera);

	// for now, move the render location to fake a scene window
	ImVec2 windowPos = ImGui::GetWindowPos();

	ImVec2 min = ImVec2(0.0f, 0.0f); //ImGui::GetWindowContentRegionMin();
	ImVec2 max = ImGui::GetWindowSize(); //ImGui::GetWindowContentRegionMax();

	ImVec2 pos = ImVec2(min.x + windowPos.x, min.y + windowPos.y);
	ImVec2 size = ImVec2(max.x - min.x, max.y - min.y);

	renderer.set_viewport(Viewport(pos.x, pos.y, size.x, size.y, 0.0f, 1.0f));

	//ImGui::Image(renderer.get_scene_descriptor_set(), size);

	// get input
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsWindowHovered())
	{
		if (!_isMouseMoving && !_isMouseRotating && ImGui::IsMouseDown(ImGuiMouseButton_Middle))
		{
			// first down
			_isMouseMoving = true;
			_mouseDragPos = io.MousePos;
		}

		if (!_isMouseMoving && !_isMouseRotating && ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			_isMouseRotating = true;
			_mouseDragPos = io.MousePos;
		}

		if (_isMouseMoving)
		{
			// move
			ImVec2 mousePos = io.MousePos;

			_cameraPosition += -1.0f * MOUSE_MOVE_SENSITIVITY * (right(_cameraRotation) * (mousePos.x - _mouseDragPos.x));
			_cameraPosition += 1.0f * MOUSE_MOVE_SENSITIVITY * (up(_cameraRotation) * (mousePos.y - _mouseDragPos.y));

			_mouseDragPos = mousePos;
		}

		if (_isMouseRotating)
		{
			// yaw is Y, pitch is X

			ImVec2 mousePos = io.MousePos;

			_cameraOrientation += MOUSE_ROTATE_SENSITIVITY * Vector3(-(mousePos.y - _mouseDragPos.y), (mousePos.x - _mouseDragPos.x), 0.0f);
			_cameraOrientation.x = Math::clamp(_cameraOrientation.x, -90.0f + 0.1f, 90.0f - 0.1f);

			_mouseDragPos = mousePos;

			// set yaw and pitch back
			_cameraRotation = from_euler(_cameraOrientation);
		}

		_cameraPosition += forward(_cameraRotation) * io.MouseWheel;
	}

	if (_isMouseMoving && !ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		// stop moving
		_isMouseMoving = false;
	}

	if (_isMouseRotating && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		// stop rotating
		_isMouseRotating = false;
	}

	ImGui::End();
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
