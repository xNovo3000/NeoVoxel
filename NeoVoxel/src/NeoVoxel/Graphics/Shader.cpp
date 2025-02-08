#include <PCH.h>
#include <NeoVoxel/Graphics/Shader.h>

namespace NeoVoxel {

	void Shader::activate() {}

	void Shader::setUniform(const std::string& locationName, int32_t value) {}
	void Shader::setUniform(const std::string& locationName, float value) {}
	void Shader::setUniform(const std::string& locationName, const glm::vec2& value) {}
	void Shader::setUniform(const std::string& locationName, const glm::vec3& value) {}
	void Shader::setUniform(const std::string& locationName, const glm::mat3& value) {}
	void Shader::setUniform(const std::string& locationName, const glm::mat4& value) {}

}