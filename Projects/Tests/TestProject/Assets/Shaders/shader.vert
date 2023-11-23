#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 transform;
} camera;

layout(set = 2, binding = 0) uniform MaterialBufferObject
{
    int textureId;
    int x, y, z;
    vec4 color;
} material;

layout(push_constant) uniform DrawObject
{
    mat4 transform;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat int fragTexId;

void main() {
    gl_Position = camera.transform * object.transform * vec4(inPosition, 1.0);
    fragColor = material.color;    
    fragTexCoord = inTexCoord;
    fragTexId = material.textureId;
}