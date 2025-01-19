#version 330 core

in vec2 p_TexCoord;

out vec4 o_FragColor;

void main() {
	o_FragColor = vec4(p_TexCoord.x, p_TexCoord.y, 0.0, 1.0);
}