#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    vec4 texColor = texture(texSampler, fragTexCoord);

    if(texColor.a == 0.0) discard;

    outColor = vec4(fragColor.rgb, texColor.a);
}