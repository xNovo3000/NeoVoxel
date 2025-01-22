#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec2 i_Position;
layout (location = 1) in vec2 i_TexCoord;

layout (location = 0) uniform mat4 u_ProjectionMatrix;
layout (location = 1) uniform mat4 u_ViewMatrix;
layout (location = 2) uniform mat4 u_ModelMatrix;

out vec2 p_TexCoord;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(i_Position, 0.0, 1.0);
	p_TexCoord = i_TexCoord;
}