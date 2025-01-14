#version 410 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec3 aPos;
layout (location = 0) out vec3 vCubeDir;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

void main() {
    vCubeDir = aPos;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
