#include <PCH.h>
#include <NeoVoxel/Graphics/Shader.h>

namespace NeoVoxel {

	void Shader::activate() {}

	void Shader::setUniform(uint32_t location, int32_t value) {}
	void Shader::setUniform(uint32_t location, float value) {}
	void Shader::setUniform(uint32_t location, const glm::vec2& value) {}
	void Shader::setUniform(uint32_t location, const glm::vec3& value) {}
	void Shader::setUniform(uint32_t location, const glm::mat3& value) {}
	void Shader::setUniform(uint32_t location, const glm::mat4& value) {}

}