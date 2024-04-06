#version 450

#define ANCHOR_MODE_ALL 0b00000000
#define ANCHOR_MODE_TOP 0b00000001
#define ANCHOR_MODE_MIDDLE 0b00000010
#define ANCHOR_MODE_BOTTOM 0b00000100
#define ANCHOR_MODE_LEFT 0b00001000
#define ANCHOR_MODE_CENTER 0b00010000
#define ANCHOR_MODE_RIGHT 0b00100000

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CanvasBufferObject {
    int width;
    int height;
	int unused0, unused1;
} canvas;

layout(push_constant) uniform UIObject
{
	float x; // left
	float y; // top
	float width; // right
	float height; // bottom
    int anchorMode;
    int unused0, unused1, unused2;
} object;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    //gl_VertexIndex
    vec2 vertices[6] =
    {
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(0.0, 1.0),
        vec2(1.0, 0.0),
        vec2(1.0, 1.0)
    };

    vec2 pos = vertices[gl_VertexIndex % 6];
    gl_Position = vec4((object.x + pos.x * object.width) / canvas.width * 2.0f - 1.0f, (object.y + pos.y * object.height) / canvas.height * 2.0f - 1.0f, 0.0, 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    fragTexCoord = pos;
}