#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uvCoordinates;
layout(location = 0) out vec4 fragColor;
layout(location = 0) uniform sampler2D tex0;
layout(location = 1) uniform float uMove;

void main() {
    fragColor = texture(tex0, uvCoordinates + uMove);
}
