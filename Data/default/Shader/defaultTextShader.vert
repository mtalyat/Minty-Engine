#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CanvasBufferObject {
    int width;
    int height;
	int unused0, unused1;
} canvas;

layout(push_constant) uniform TextObject
{
    float x; // left
	float y; // top
	float width; // right
	float height; // bottom
    vec4 color;
    int anchorMode;
    int unused0, unused1, unused2;
} object;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4((object.x + inPosition.x * object.width) / canvas.width * 2.0 - 1.0, (object.y + inPosition.y * object.height) / canvas.height * 2.0 - 1.0, 0.0, 1.0);
    fragColor = vec4(object.color.rgb, 1.0);
    fragTexCoord = inTexCoord;
}