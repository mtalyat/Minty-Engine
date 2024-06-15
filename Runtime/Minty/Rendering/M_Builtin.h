#pragma once

#include "Minty/Types/M_Matrix.h"
#include "Minty/Types/M_Vector.h"

namespace Minty::Builtin
{
	// global uniform buffer object
	struct GlobalUBO
	{
		alignas(16) float time;
		float deltaTime;
		float unused0;
		float unused1;

		alignas(16) Matrix4 view;
	};

	struct Vertex3D
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 coord;
	};

	struct Vertex2D
	{
		Vector2 pos;
		Vector2 coord;
	};

	struct UIPushData
	{
		alignas(16) float x; // left
		float y; // top
		float width; // right
		float height; // bottom
		Vector4 color;
		alignas(16) int anchorMode;
		int unused0, unused1, unused2;
	};

	struct CanvasBufferObject
	{
		alignas(16) int width;
		int height;
		int unused0, unused1;
	};

	struct UITextPushData
	{
		alignas(16) float x; // left
		float y; // top
		float width; // right
		float height; // bottom
		Vector4 color;
		alignas(16) int anchorMode;
		int unused0, unused1, unused2;
	};
}