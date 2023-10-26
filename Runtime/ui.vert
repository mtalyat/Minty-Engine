#version 450

#extension GL_EXT_nonuniform_qualifier : require

struct Material
{
    int textureId;
    int x, y, z;
    vec4 color;
};

layout(set = 0, binding = 2) uniform MaterialBufferObjects
{
    Material materials[4];
} materials;

layout(push_constant) uniform DrawObject
{
    int materialId;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat int fragTexId;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = materials.materials[object.materialId].color;
    fragTexCoord = inTexCoord;
    fragTexId = materials.materials[object.materialId].textureId;
}