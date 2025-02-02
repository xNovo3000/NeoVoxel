#version 330 core

layout (location = 0) in vec2 i_Position;
layout (location = 1) in vec2 i_TexCoord;

out vec2 p_TexCoord;

void main() {
	gl_Position = vec4(i_Position, 0.0, 1.0);
	p_TexCoord = i_TexCoord;
}