#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(binding = 0) uniform Camera {
    mat4 camera;
} camera;

layout(location = 0) in vec4 instanceColor;
layout(location = 1) in vec2 instanceOffset;
layout(location = 2) in vec2 instanceSize;
layout(location = 3) in vec2 instancePivot;
layout(location = 4) in float instanceScale;
layout(location = 5) in mat4 instanceTransform;

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
    
    gl_Position = camera.camera * instanceTransform * vec4((pos.x - instancePivot.x) * instanceScale, (pos.y - instancePivot.y) * instanceScale, 0.0, 1.0);
    fragColor = instanceColor;
    fragTexCoord = pos * instanceSize + instanceOffset;
}