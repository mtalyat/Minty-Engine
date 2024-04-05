#version 450

#define ANCHOR_MODE_ALL 0b00000000

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CanvasBufferObject {
    int width;
    int height;
    int resolutionWidth;
    int resolutionHeight;
} camera;

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
    // get the X/Y based on the anchor mode
    vec2 pos = vertices[gl_VertexIndex % 6];
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    fragTexCoord = pos;
}