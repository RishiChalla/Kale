#version 450

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) uniform vec4 vertexColor;
layout(location = 2) uniform float zPosition;

out vec4 fragColor;

void main() {
    gl_Position = vec4(vertexPosition, zPosition, 1.0);
	fragColor = vertexColor;
}
