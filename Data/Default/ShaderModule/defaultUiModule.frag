#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 2, binding = 0) uniform sampler2D texSampler;

void main() {
    outColor = fragColor * texture(texSampler, fragTexCoord);

    // ignore invisible pixels
    if(outColor.a == 0) discard;
}