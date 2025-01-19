#include <PCH.h>
#include "OpenGLShader.h"
#include "OpenGLErrorManagement.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#include <glm/gtc/type_ptr.hpp>

namespace NeoVoxel {

	/* Utility */

#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
	static inline void utility_CheckShaderValidity(GLuint shaderId) {
		int hasCompiledSuccessfully = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiledSuccessfully);
		if (hasCompiledSuccessfully == GL_FALSE) {
			// Print why
			int infoLogLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
			auto errorLog = std::make_unique<char[]>(infoLogLength);
			glGetShaderInfoLog(shaderId, infoLogLength, &infoLogLength, errorLog.get());
			NV_ERROR("OpenGL shader compilation error: {}", errorLog.get());
		}
	}
#endif

	/* OpenGLShader */

	OpenGLShader::OpenGLShader(const ShaderSpec& spec) : Shader(), m_ProgramHandle(OPENGL_INVALID_HANDLE) {
		NV_PROFILE;
		// Create vertex shader
		auto vertexShaderHandle = glCall(glCreateShader(GL_VERTEX_SHADER));
		auto vertexShaderSourcePtr = spec.m_VertexShader.c_str();
		glCall(glShaderSource(vertexShaderHandle, 1, &vertexShaderSourcePtr, nullptr));
		glCall(glCompileShader(vertexShaderHandle));
#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
		utility_CheckShaderValidity(vertexShaderHandle);
#endif
		// Create geometry shader (optional)
		GLuint geometryShaderHandle;
		if (spec.m_GeometryShader.has_value()) {
			geometryShaderHandle = glCall(glCreateShader(GL_GEOMETRY_SHADER));
			auto geometryShaderSourcePtr = (*spec.m_GeometryShader).c_str();
			glCall(glShaderSource(geometryShaderHandle, 1, &geometryShaderSourcePtr, nullptr));
			glCall(glCompileShader(geometryShaderHandle));
#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
			utility_CheckShaderValidity(geometryShaderHandle);
#endif
		}
		// Create fragment shader
		auto fragmentShaderHandle = glCall(glCreateShader(GL_FRAGMENT_SHADER));
		auto fragmentShaderSourcePtr = spec.m_FragmentShader.c_str();
		glCall(glShaderSource(fragmentShaderHandle, 1, &fragmentShaderSourcePtr, nullptr));
		glCall(glCompileShader(fragmentShaderHandle));
#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
		utility_CheckShaderValidity(fragmentShaderHandle);
#endif
		// Create program
		m_ProgramHandle = glCall(glCreateProgram());
		// Attach shaders to program
		glCall(glAttachShader(m_ProgramHandle, vertexShaderHandle));
		if (spec.m_GeometryShader.has_value()) {
			glCall(glAttachShader(m_ProgramHandle, geometryShaderHandle));
		}
		glCall(glAttachShader(m_ProgramHandle, fragmentShaderHandle));
		// Link program
		glLinkProgram(m_ProgramHandle);
		// Detach shaders from program
		glCall(glDetachShader(m_ProgramHandle, vertexShaderHandle));
		if (spec.m_GeometryShader.has_value()) {
			glCall(glDetachShader(m_ProgramHandle, geometryShaderHandle));
		}
		glCall(glDetachShader(m_ProgramHandle, fragmentShaderHandle));
		// Delete shaders
		glCall(glDeleteShader(vertexShaderHandle));
		if (spec.m_GeometryShader.has_value()) {
			glCall(glDeleteShader(geometryShaderHandle));
		}
		glCall(glDeleteShader(fragmentShaderHandle));
	}

	OpenGLShader::~OpenGLShader() {
		if (m_ProgramHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
			glCall(glDeleteProgram(m_ProgramHandle));
		}
	}

	OpenGLShader::OpenGLShader(OpenGLShader&& other) noexcept :
		Shader(std::move(other)), m_ProgramHandle(other.m_ProgramHandle)
	{
		other.m_ProgramHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLShader& OpenGLShader::operator=(OpenGLShader&& other) noexcept {
		Shader::operator=(std::move(other));
		m_ProgramHandle = other.m_ProgramHandle;
		other.m_ProgramHandle = OPENGL_INVALID_HANDLE;
		return *this;
	}

	void OpenGLShader::activate() {
		NV_PROFILE;
		glCall(glUseProgram(m_ProgramHandle));
	}

	void OpenGLShader::setUniform(uint32_t location, int32_t value) {
		NV_PROFILE;
		glCall(glUniform1i(location, value));
	}

	void OpenGLShader::setUniform(uint32_t location, float value) {
		NV_PROFILE;
		glCall(glUniform1f(location, value));
	}

	void OpenGLShader::setUniform(uint32_t location, const glm::vec2& value) {
		NV_PROFILE;
		glCall(glUniform2fv(location, 1, glm::value_ptr(value)));
	}

	void OpenGLShader::setUniform(uint32_t location, const glm::vec3& value) {
		NV_PROFILE;
		glCall(glUniform3fv(location, 1, glm::value_ptr(value)));
	}

	void OpenGLShader::setUniform(uint32_t location, const glm::mat3& value) {
		NV_PROFILE;
		glCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
	}

	void OpenGLShader::setUniform(uint32_t location, const glm::mat4& value) {
		NV_PROFILE;
		glCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
	}

}