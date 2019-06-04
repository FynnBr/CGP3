#version 410 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec3 vCubeDir;

layout (location = 0) out vec4 fragColor;

layout (location = 8) uniform samplerCube uSkybox;;

void main() {
    fragColor = texture(uSkybox, vCubeDir);
}
