#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat int fragTexId;

layout(location = 0) out vec4 outColor;

layout(set = 2, binding = 1) uniform sampler2D texSamplers[];

void main() {
    // outColor = vec4(1.0, 0.0, 0.0, 1.0);
    // outColor = vec4(fragColor.xyz, 1.0);
    // outColor = fragColor * texture(texSamplers[0], fragTexCoord);
    // outColor = texture(texSamplers[nonuniformEXT(fragTexId)], fragTexCoord);
    outColor = fragColor * texture(texSamplers[nonuniformEXT(fragTexId)], fragTexCoord);
}