#include <PCH.h>
#include "OpenGLTexture.h"
#include "OpenGLErrorManagement.h"
#include "OpenGLMemoryProfile.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {

	/* Utility */

	static inline GLenum utility_GetParameterName(TextureParamsName name) {
		switch (name) {
			case TextureParamsName::WRAP_S: return GL_TEXTURE_WRAP_S;
			case TextureParamsName::WRAP_T: return GL_TEXTURE_WRAP_T;
			case TextureParamsName::FILTER_MIN: return GL_TEXTURE_MIN_FILTER;
			case TextureParamsName::FILTER_MAG: return GL_TEXTURE_MAG_FILTER;
		}
		return GL_TEXTURE_WRAP_S;
	}

	static inline GLint utility_GetParameterValue(
		TextureParamsName name,
		TextureParamsValue value,
		TextureMipmapGeneration generateMipmaps
	) {
		if (generateMipmaps == TextureMipmapGeneration::ENABLED && name == TextureParamsName::FILTER_MIN) {
			switch (value) {
				case TextureParamsValue::FILTER_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
				case TextureParamsValue::FILTER_LINEAR: return GL_LINEAR_MIPMAP_NEAREST;
			}
			return GL_NEAREST_MIPMAP_NEAREST;
		}
		else {
			switch (value) {
				case TextureParamsValue::WRAP_CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
				case TextureParamsValue::WRAP_REPEAT: return GL_REPEAT;
				case TextureParamsValue::FILTER_NEAREST: return GL_NEAREST;
				case TextureParamsValue::FILTER_LINEAR: return GL_LINEAR;
			}
			return GL_CLAMP_TO_EDGE;
		}
	}

	static inline GLint utility_GetInternalFormat(TextureColorSpace colorSpace) {
		switch (colorSpace) {
			case TextureColorSpace::SDR: return GL_RGBA8;
			case TextureColorSpace::HDR16: return GL_RGBA16F;
			case TextureColorSpace::HDR32: return GL_RGBA32F;
		}
		return GL_RGBA8;
	}

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	static inline uint32_t utility_GetInternalFormatSize(TextureColorSpace colorSpace) {
		switch (colorSpace) {
			case TextureColorSpace::SDR: return 4;
			case TextureColorSpace::HDR16: return 8;
			case TextureColorSpace::HDR32: return 16;
		}
		return 0;
	}

	static inline uint32_t utility_GenerateMipmapMultiplier(TextureMipmapGeneration generateMipmaps) {
		switch (generateMipmaps) {
			case TextureMipmapGeneration::DISABLED: return 1;
			case TextureMipmapGeneration::ENABLED: return 2;
		}
		return 0;
	}

#endif

	/* OpenGLTexture2D */

	OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpec& spec) :
		Texture2D(), m_TextureHandle(OPENGL_INVALID_HANDLE),
		m_ColorSpace(spec.m_ColorSpace), m_GenerateMipmaps(spec.m_MipmapGeneration)
	{
		NV_PROFILE;
		// Generate texture
		glCall(glGenTextures(1, &m_TextureHandle));
		glCall(glBindTexture(GL_TEXTURE_2D, m_TextureHandle));
		// Set parameters
		for (auto& [name, value] : spec.m_Parameters) {
			auto parameterName = utility_GetParameterName(name);
			auto parameterValue = utility_GetParameterValue(name, value, m_GenerateMipmaps);
			glCall(glTexParameteri(GL_TEXTURE_2D, parameterName, parameterValue));
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		if (m_TextureHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
			glCall(glDeleteTextures(1, &m_TextureHandle));
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept :
		Texture2D(std::move(other)), m_TextureHandle(other.m_TextureHandle),
		m_ColorSpace(other.m_ColorSpace), m_GenerateMipmaps(other.m_GenerateMipmaps)
	{
		other.m_TextureHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLTexture2D& OpenGLTexture2D::operator=(OpenGLTexture2D&& other) noexcept {
		Texture2D::operator=(std::move(other));
		m_TextureHandle = other.m_TextureHandle;
		m_ColorSpace = other.m_ColorSpace;
		m_GenerateMipmaps = other.m_GenerateMipmaps;
		other.m_TextureHandle = OPENGL_INVALID_HANDLE;
		return *this;
	}

	void OpenGLTexture2D::bind(uint32_t slot) {
		NV_PROFILE;
		glCall(glActiveTexture(GL_TEXTURE0 + slot));
		glCall(glBindTexture(GL_TEXTURE_2D, m_TextureHandle));
	}

	void OpenGLTexture2D::update(const glm::ivec2& size) {
		NV_PROFILE;
		NV_PROFILE_MEMORY(m_TextureHandle, size.x * size.y * utility_GetInternalFormatSize(m_ColorSpace));
		glCall(glBindTexture(GL_TEXTURE_2D, m_TextureHandle));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, utility_GetInternalFormat(m_ColorSpace), size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
	}

	void OpenGLTexture2D::update(const glm::ivec2& size, const std::vector<uint8_t>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY(m_TextureHandle, size.x * size.y * utility_GetInternalFormatSize(m_ColorSpace) * utility_GenerateMipmapMultiplier(m_GenerateMipmaps));
		glCall(glBindTexture(GL_TEXTURE_2D, m_TextureHandle));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, utility_GetInternalFormat(m_ColorSpace), size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data()));
		// Generate mipmaps (if required)
		if (m_GenerateMipmaps == TextureMipmapGeneration::ENABLED) {
			glCall(glGenerateMipmap(GL_TEXTURE_2D));
		}
	}

	void OpenGLTexture2D::subUpdate(const glm::ivec2& size, const glm::ivec2& offset, const std::vector<uint8_t>& data) {
		NV_PROFILE;
		glCall(glBindTexture(GL_TEXTURE_2D, m_TextureHandle));
		glCall(glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data()));
		// Generate mipmaps (if required)
		if (m_GenerateMipmaps == TextureMipmapGeneration::ENABLED) {
			glCall(glGenerateMipmap(GL_TEXTURE_2D));
		}
	}

}