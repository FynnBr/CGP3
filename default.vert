#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 inCoordinates;
layout(location = 0) out vec2 outCoordinates;
layout(location = 1) out vec3 vColor;

void main() {
    // pass color to fragment shader
    vColor = aColor;
    outCoordinates = inCoordinates;
    // define vertex position in space.
    // must be 4-dimensional, so supplement Z and W values.
    gl_Position = vec4(aPosition, 0.0f, 1.0f);
}

