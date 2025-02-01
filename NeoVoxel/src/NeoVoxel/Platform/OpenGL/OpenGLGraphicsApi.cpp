#include <PCH.h>
#include "OpenGLGraphicsApi.h"
#include "OpenGLErrorManagement.h"
#include "OpenGLArrayBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

namespace NeoVoxel {

	/* OpenGL - Debug callback */

	static inline const char* callback_Helper_GetSeverityString(GLenum severity) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_LOW_ARB: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM_ARB: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH_ARB: return "HIGH";
		}
		return "INVALID";
	}

	static inline const char* callback_Helper_GetTypeString(GLenum type) {
		switch (type) {
			case GL_DEBUG_TYPE_ERROR_ARB: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY_ARB: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE_ARB: return "PERFORMANCE";
			case GL_DEBUG_TYPE_OTHER_ARB: return "OTHER";
		}
		return "INVALID";
	}

	static inline const char* callback_Helper_GetSourceString(GLenum type) {
		switch (type) {
			case GL_DEBUG_SOURCE_API_ARB: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: return "WINDOW_SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: return "SHADER_COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: return "THIRD_PARTY";
			case GL_DEBUG_SOURCE_APPLICATION_ARB: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER_ARB: return "OTHER";
		}
		return "INVALID";
	}

	static void callback_OpenGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* ptr) {
		NV_ERROR(
			"OpenGL Error {}.\n\tSeverity: {}\n\tSource: {}\n\tType: {}\n{}",
			id, callback_Helper_GetSeverityString(severity), callback_Helper_GetSourceString(source),
			callback_Helper_GetTypeString(type), message
		);
	}

	/* OpenGLGraphicsApi */

	OpenGLGraphicsApi::OpenGLGraphicsApi(OpenGLLoaderPtr loaderFunction) : GraphicsApi("OpenGLGraphicsApi") {
		// Load OpenGL
		if (!gladLoadGL(loaderFunction)) {
			NV_CRITICAL("{}: cannot load OpenGL APIs", getName());
		}
		// Check for extensions
		if (!GLAD_GL_ARB_explicit_uniform_location) {
			NV_CRITICAL("{}: required extension GLAD_GL_ARB_explicit_uniform_location not found");
		}
		// Enable debug output if requested
#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
		if (GLAD_GL_ARB_debug_output) {
			glDebugMessageCallbackARB(callback_OpenGLError, nullptr);
		}
		else {
			NV_ERROR("{}: missing extension GLAD_GL_ARB_debug_output. Cannot initialize OpenGL debug output", getName());
		}
#endif
	}

	void OpenGLGraphicsApi::clearColor(const glm::vec4& color) {
		NV_PROFILE;
		glCall(glClearColor(color.r, color.g, color.b, color.a));
		glCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void OpenGLGraphicsApi::clearDepth(float depth) {
		NV_PROFILE;
		glCall(glClearDepth(depth));
		glCall(glClear(GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLGraphicsApi::setViewport(const glm::ivec2& viewport) {
		NV_PROFILE;
		glCall(glViewport(0, 0, viewport.x, viewport.y));
	}

	void OpenGLGraphicsApi::unbindFramebuffer() {
		NV_PROFILE;
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	ArrayBufferRef OpenGLGraphicsApi::createArrayBuffer(const ArrayBufferSpec& spec) {
		return std::make_shared<OpenGLArrayBuffer>(spec);
	}

	FramebufferRef OpenGLGraphicsApi::createFramebuffer(const FramebufferSpec& spec) {
		return std::make_shared<OpenGLFramebuffer>(spec);
	}

	ShaderRef OpenGLGraphicsApi::createShader(const ShaderSpec& spec) {
		return std::make_shared<OpenGLShader>(spec);
	}

	Texture2DRef OpenGLGraphicsApi::createTexture2D(const Texture2DSpec& spec) {
		return std::make_shared<OpenGLTexture2D>(spec);
	}

}