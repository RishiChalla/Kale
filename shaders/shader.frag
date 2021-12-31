#version 450

layout(location = 3) in vec4 fragColor;
layout(location = 4) out vec4 outColor;

void main() {
    outColor = fragColor;
}
