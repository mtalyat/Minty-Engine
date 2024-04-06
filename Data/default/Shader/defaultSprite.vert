#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 transform;
} camera;

layout(push_constant) uniform SpriteObject
{
    mat4 transform;
    vec4 color;
    vec2 minCoords;
    vec2 maxCoords;
    vec2 pivot;
    vec2 size;
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
    gl_Position = camera.transform * object.transform * vec4((pos.x - object.pivot.x) * object.size.x, (pos.y - object.pivot.y) * object.size.y, 0.0, 1.0);
    fragColor = object.color;
    fragTexCoord = pos * (object.maxCoords - object.minCoords) + object.minCoords;
}