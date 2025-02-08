#pragma once

#include <NeoVoxel/Graphics/Shader.h>

#include <glad/gl.h>

namespace NeoVoxel {

	class OpenGLShader : public Shader {

	public:
		OpenGLShader(const ShaderSpec& spec);
		virtual ~OpenGLShader();

		OpenGLShader(const OpenGLShader&) = delete;
		OpenGLShader& operator=(const OpenGLShader&) = delete;

		OpenGLShader(OpenGLShader&& other) noexcept;
		OpenGLShader& operator=(OpenGLShader&& other) noexcept;

		virtual void activate() override;

		virtual void setUniform(const std::string& locationName, int32_t value) override;
		virtual void setUniform(const std::string& locationName, float value) override;
		virtual void setUniform(const std::string& locationName, const glm::vec2& value) override;
		virtual void setUniform(const std::string& locationName, const glm::vec3& value) override;
		virtual void setUniform(const std::string& locationName, const glm::mat3& value) override;
		virtual void setUniform(const std::string& locationName, const glm::mat4& value) override;

	private:
		GLuint m_ProgramHandle;

	};

}