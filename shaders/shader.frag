#version 450

layout(location = 2) in vec3 fragColor;
layout(location = 3) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
