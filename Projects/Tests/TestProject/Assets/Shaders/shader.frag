#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat int fragTexId;

layout(location = 0) out vec4 outColor;

layout(set = 2, binding = 1) uniform sampler2D texSamplers[];

void main() {
    outColor = fragColor * texture(texSamplers[nonuniformEXT(fragTexId)], fragTexCoord);
    // outColor = vec4(1.0, 0.0, 0.0, 1.0); // red
    // outColor = vec4(fragColor.xyz, 1.0); // material color
    // outColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.0f, 1.0f); // x = red, y = green
    // outColor = fragColor;
}