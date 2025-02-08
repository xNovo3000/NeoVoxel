#version 330 core

in vec2 p_TexCoord;

out vec4 o_FragColor;

uniform sampler2D u_Result0;
uniform vec2 u_Offset;
uniform float u_Gamma;
uniform mat3 u_Kernel;
uniform float u_Exposure;

void main() {
	
	vec3 textureColor[9];
	vec3 finalColor = vec3(0.0);

	// Extract current and neighbors texture RGB value
	textureColor[0] = vec3(texture(u_Result0, p_TexCoord + vec2(-u_Offset.x,  u_Offset.y)));
	textureColor[1] = vec3(texture(u_Result0, p_TexCoord + vec2( 0.0,         u_Offset.y)));
	textureColor[2] = vec3(texture(u_Result0, p_TexCoord + vec2( u_Offset.x,  u_Offset.y)));
	textureColor[3] = vec3(texture(u_Result0, p_TexCoord + vec2(-u_Offset.x,  0.0       )));
	textureColor[4] = vec3(texture(u_Result0, p_TexCoord + vec2( 0.0,         0.0       )));
	textureColor[5] = vec3(texture(u_Result0, p_TexCoord + vec2( u_Offset.x,  0.0       )));
	textureColor[6] = vec3(texture(u_Result0, p_TexCoord + vec2(-u_Offset.x, -u_Offset.y)));
	textureColor[7] = vec3(texture(u_Result0, p_TexCoord + vec2( 0.0,        -u_Offset.y)));
	textureColor[8] = vec3(texture(u_Result0, p_TexCoord + vec2( u_Offset.x, -u_Offset.y)));

	// Apply kernel to the current fragment color
	finalColor += textureColor[0] * u_Kernel[0][0];
	finalColor += textureColor[1] * u_Kernel[1][0];
	finalColor += textureColor[2] * u_Kernel[2][0];
	finalColor += textureColor[3] * u_Kernel[0][1];
	finalColor += textureColor[4] * u_Kernel[1][1];
	finalColor += textureColor[5] * u_Kernel[2][1];
	finalColor += textureColor[6] * u_Kernel[0][2];
	finalColor += textureColor[7] * u_Kernel[1][2];
	finalColor += textureColor[8] * u_Kernel[2][2];

	// Exposure tone mapping
	finalColor = vec3(1.0) - exp(-finalColor * u_Exposure);

	// Gamma correction
	finalColor = pow(finalColor, vec3(1.0 / u_Gamma));

	o_FragColor = vec4(finalColor, 1.0);

}