#version 450

uniform mat3 camera;
uniform mat3 local;
uniform vec4 vertexColor;
uniform float zPosition;

in vec2 vertexPosition;

out vec4 fragColor;

vec2 transform(mat3 mat, vec2 vert) {
	return vec2(
		mat[0][0] * vert.x + mat[0][1] * vert.y + mat[0][2],
		mat[1][0] * vert.x + mat[1][1] * vert.y + mat[1][2]
	);
}

void main() {
    gl_Position = vec4(transform(camera, transform(local, vertexPosition)), zPosition, 1.0);
	fragColor = vertexColor;
}
