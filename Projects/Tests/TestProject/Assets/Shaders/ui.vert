#version 450

#extension GL_EXT_nonuniform_qualifier : require

struct Material
{
    int textureId;
    int x, y, z;
    vec4 color;
};

layout(set = 0, binding = 1) uniform MaterialBufferObjects
{
    Material materials[8];
} materials;

layout(push_constant) uniform DrawObject
{
    int materialId;
    int layer;
    int x, y;
    vec2 minCoords;
    vec2 maxCoords;
    vec2 minPos;
    vec2 maxPos;
} object;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat int fragTexId;

void main() {
    //gl_VertexIndex
    vec2 pos;
    switch(gl_VertexIndex)
    {
        case 0:
        pos = vec2(0.0, 0.0);
        break;
        case 1:
        case 4:
        pos = vec2(1.0, 0.0);
        break;
        case 2:
        case 3:
        pos = vec2(0.0, 1.0);
        break;
        case 5:
        pos = vec2(1.0, 1.0);
        break;
    }
    gl_Position = vec4(pos * (object.maxPos - object.minPos) * 2.0 + object.minPos * 2.0 - 1.0, 0.5 + object.layer * 0.00001, 1.0);
    fragColor = materials.materials[object.materialId].color;
    fragTexCoord = pos * (object.maxCoords - object.minCoords) + object.minCoords;
    fragTexId = materials.materials[object.materialId].textureId;
}