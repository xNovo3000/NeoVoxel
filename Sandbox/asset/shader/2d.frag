#version 330 core

in vec2 p_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Diffuse0;

void main() {
	o_FragColor = texture(u_Diffuse0, p_TexCoord);
}