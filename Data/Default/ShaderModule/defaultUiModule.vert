#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(binding = 0) uniform Canvas {
    int width;
    int height;
    int unused0;
    int unused1;
} canvas;

layout(location = 0) in vec4 position; // (x, y, width, height)
layout(location = 1) in vec4 uv; // (x, y, width, height)
layout(location = 2) in vec4 color;

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
    gl_Position = vec4((position.x + pos.x * position.z) / canvas.width * 2.0 - 1.0, (position.y + pos.y * position.w) / canvas.height * 2.0 - 1.0, 0.0, 1.0);
    fragColor = color;
    fragTexCoord = pos;
}