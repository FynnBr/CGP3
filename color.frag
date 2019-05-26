#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec3 vColor;
// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;
layout(location = 0) uniform float uAlpha = 1;

void main() {
    // write interplated color to fragment, set alpha = 1.0
    fragColor = vec4(vColor, uAlpha);
}
