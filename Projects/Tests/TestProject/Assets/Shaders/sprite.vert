#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 transform;
} camera;

layout(push_constant) uniform SpriteObject
{
    mat4 transform;
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
    gl_Position = camera.transform * object.transform * vec4(pos.x - object.pivot.x * object.size.x, pos.y - object.pivot.y * object.size.y, 0.0, 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    fragTexCoord = pos * (object.maxCoords - object.minCoords) + object.minCoords;

    // from ui.vert:
    // gl_Position = vec4(pos * (object.maxPos - object.minPos) * 2.0 + object.minPos * 2.0 - 1.0, 0.0, 1.0);
    // fragColor = materials.materials[object.materialId].color;
    // fragTexCoord = pos * (object.maxCoords - object.minCoords) + object.minCoords;
    // fragTexId = materials.materials[object.materialId].textureId;

    // from shader.vert:
    // gl_Position = camera.transform * object.transform * vec4(inPosition.x - sprite.pivot.x * sprite.size.x, inPosition.y - sprite.pivot.y * sprite.pivot.y, inPosition.z, 1.0);
    // fragColor = sprite.color;
    // fragTexCoord = inTexCoord;
}