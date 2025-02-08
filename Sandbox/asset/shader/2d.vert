#version 330 core

layout (location = 0) in vec2 i_Position;
layout (location = 1) in vec2 i_TexCoord;

uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_ModelMatrix;

out vec2 p_TexCoord;

void main() {
	gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(i_Position, 0.0, 1.0);
	p_TexCoord = i_TexCoord;
}