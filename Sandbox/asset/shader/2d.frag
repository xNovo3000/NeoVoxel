#version 330 core
#extension GL_ARB_explicit_uniform_location : require

in vec2 p_TexCoord;

out vec4 o_FragColor;

layout (location = 2) uniform sampler2D u_Texture;

void main() {
	o_FragColor = texture(u_Texture, p_TexCoord);
}