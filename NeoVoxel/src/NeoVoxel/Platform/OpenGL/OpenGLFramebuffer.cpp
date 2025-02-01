#include <PCH.h>
#include "OpenGLFramebuffer.h"
#include "OpenGLErrorManagement.h"
#include "OpenGLMemoryProfile.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {

	/* Utility */

	static inline GLenum utility_GetInternalFormat(FramebufferColorChannels channels) {
		switch (channels) {
			case FramebufferColorChannels::RGB_8: return GL_RGB8;
			case FramebufferColorChannels::RGBA_8: return GL_RGBA8;
			case FramebufferColorChannels::RGB_16: return GL_RGB16F;
			case FramebufferColorChannels::RGBA_16: return GL_RGBA16F;
		}
		return GL_RGB8;
	}

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	static inline uint32_t utility_GetBytesPerColorChannels(FramebufferColorChannels channels) {
		switch (channels) {
			case FramebufferColorChannels::RGB_8: return 3;
			case FramebufferColorChannels::RGBA_8: return 4;
			case FramebufferColorChannels::RGB_16: return 6;
			case FramebufferColorChannels::RGBA_16: return 8;
		}
		return 0;
	}

	static inline uint32_t utility_GetBytesPerColorType(FramebufferColorType type) {
		switch (type) {
			case FramebufferColorType::ABSENT: return 0;
			case FramebufferColorType::PRESENT:
			case FramebufferColorType::PRESENT_BINDABLE: return 1;
		}
		return 0;
	}

	static inline uint32_t utility_GetBytesPerDepthType(FramebufferDepthType type) {
		switch (type) {
			case FramebufferDepthType::ABSENT: return 0;
			case FramebufferDepthType::PRESENT:
			case FramebufferDepthType::PRESENT_BINDABLE: return 1;
		}
		return 0;
	}

#endif

	/* OpenGLFramebuffer */

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec) : Framebuffer(),
		m_FramebufferHandle(OPENGL_INVALID_HANDLE), m_ColorAttachmentHandle(OPENGL_INVALID_HANDLE),
		m_DepthStencilAttachmentHandle(OPENGL_INVALID_HANDLE), m_Channels(spec.m_Channels),
		m_ColorType(spec.m_ColorType), m_DepthType(spec.m_DepthType)
	{
		NV_PROFILE;
		NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, spec.m_Size.x * spec.m_Size.y * (utility_GetBytesPerDepthType(m_DepthType) * 4 + utility_GetBytesPerColorType(m_ColorType) * utility_GetBytesPerColorChannels(m_Channels)));
		// Color attachment
		switch (m_ColorType) {
			case FramebufferColorType::ABSENT:
				break;
			case FramebufferColorType::PRESENT:
				// Create renderbuffer
				glCall(glGenRenderbuffers(1, &m_ColorAttachmentHandle));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ColorAttachmentHandle));
				glCall(glRenderbufferStorage(GL_RENDERBUFFER, utility_GetInternalFormat(m_Channels), spec.m_Size.x, spec.m_Size.y));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
				break;
			case FramebufferColorType::PRESENT_BINDABLE:
				// Create texture
				glCall(glGenTextures(1, &m_ColorAttachmentHandle));
				glCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentHandle));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				glCall(glTexImage2D(GL_TEXTURE_2D, 0, utility_GetInternalFormat(m_Channels), spec.m_Size.x, spec.m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
				glCall(glBindTexture(GL_TEXTURE_2D, 0));
				break;
		}
		// Depth-stencil attachment
		switch (m_DepthType) {
			case FramebufferDepthType::ABSENT:
				break;
			case FramebufferDepthType::PRESENT:
				// Create renderbuffer
				glCall(glGenRenderbuffers(1, &m_DepthStencilAttachmentHandle));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachmentHandle));
				glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, spec.m_Size.x, spec.m_Size.y));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
				break;
			case FramebufferDepthType::PRESENT_BINDABLE:
				// Create texture
				glCall(glGenTextures(1, &m_DepthStencilAttachmentHandle));
				glCall(glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachmentHandle));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec.m_Size.x, spec.m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
				glCall(glBindTexture(GL_TEXTURE_2D, 0));
				break;
		}
		// Framebuffer
		glCall(glGenFramebuffers(1, &m_FramebufferHandle));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		switch (m_ColorType) {
			case FramebufferColorType::ABSENT:
				break;
			case FramebufferColorType::PRESENT:
				glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorAttachmentHandle));
				break;
			case FramebufferColorType::PRESENT_BINDABLE:
				glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentHandle, 0));
				break;
		}
		switch (m_DepthType) {
			case FramebufferDepthType::ABSENT:
				break;
			case FramebufferDepthType::PRESENT:
				glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachmentHandle));
				break;
			case FramebufferDepthType::PRESENT_BINDABLE:
				glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAttachmentHandle, 0));
				break;
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			NV_ERROR("OpenGLFramebuffer: {} error", m_FramebufferHandle);
		}
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		if (m_FramebufferHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
			NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, 0);
			switch (m_ColorType) {
				case FramebufferColorType::ABSENT:
					break;
				case FramebufferColorType::PRESENT:
					glCall(glDeleteRenderbuffers(1, &m_ColorAttachmentHandle));
					break;
				case FramebufferColorType::PRESENT_BINDABLE:
					glCall(glDeleteTextures(1, &m_ColorAttachmentHandle));
					break;
			}
			switch (m_DepthType) {
				case FramebufferDepthType::ABSENT:
					break;
				case FramebufferDepthType::PRESENT:
					glCall(glDeleteRenderbuffers(1, &m_DepthStencilAttachmentHandle));
					break;
				case FramebufferDepthType::PRESENT_BINDABLE:
					glCall(glDeleteTextures(1, &m_DepthStencilAttachmentHandle));
					break;
			}
			glCall(glDeleteFramebuffers(1, &m_FramebufferHandle));
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(OpenGLFramebuffer&& other) noexcept : Framebuffer(std::move(other)),
		m_FramebufferHandle(other.m_FramebufferHandle), m_ColorAttachmentHandle(other.m_ColorAttachmentHandle),
		m_DepthStencilAttachmentHandle(other.m_DepthStencilAttachmentHandle), m_Channels(other.m_Channels),
		m_ColorType(other.m_ColorType), m_DepthType(other.m_DepthType)
	{
		other.m_FramebufferHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLFramebuffer& OpenGLFramebuffer::operator=(OpenGLFramebuffer&& other) noexcept {
		Framebuffer::operator=(std::move(other));
		m_FramebufferHandle = other.m_FramebufferHandle;
		m_ColorAttachmentHandle = other.m_ColorAttachmentHandle;
		m_DepthStencilAttachmentHandle = other.m_DepthStencilAttachmentHandle;
		m_Channels = other.m_Channels;
		m_ColorType = other.m_ColorType;
		m_DepthType = other.m_DepthType;
		other.m_FramebufferHandle = OPENGL_INVALID_HANDLE;
		return *this;
	}

	void OpenGLFramebuffer::bind(FramebufferBinding binding) {
		NV_PROFILE;
		switch (binding) {
			case FramebufferBinding::READ:
				glCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferHandle));
				break;
			case FramebufferBinding::WRITE:
				glCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FramebufferHandle));
				break;
			case FramebufferBinding::ALL:
				glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
				break;
		}
	}

	void OpenGLFramebuffer::bindColorAttachment(uint32_t slot) {
		if (m_ColorType == FramebufferColorType::PRESENT_BINDABLE) {
			NV_PROFILE;
			glCall(glActiveTexture(GL_TEXTURE0 + slot));
			glCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentHandle));
		}
		else {
			NV_WARN("OpenGLFramebuffer: {} color attachment is unbindable");
		}
	}

	void OpenGLFramebuffer::bindDepthAttachment(uint32_t slot) {
		if (m_DepthType == FramebufferDepthType::PRESENT_BINDABLE) {
			NV_PROFILE;
			glCall(glActiveTexture(GL_TEXTURE0 + slot));
			glCall(glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachmentHandle));
		}
		else {
			NV_WARN("OpenGLFramebuffer: {} depth attachment is unbindable");
		}
	}

	void OpenGLFramebuffer::setSize(const glm::ivec2& size) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, size.x * size.y * (utility_GetBytesPerDepthType(m_DepthType) * 4 + utility_GetBytesPerColorType(m_ColorType) * utility_GetBytesPerColorChannels(m_Channels)));
		switch (m_ColorType) {
			case FramebufferColorType::ABSENT:
				break;
			case FramebufferColorType::PRESENT:
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ColorAttachmentHandle));
				glCall(glRenderbufferStorage(GL_RENDERBUFFER, utility_GetInternalFormat(m_Channels), size.x, size.y));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
				break;
			case FramebufferColorType::PRESENT_BINDABLE:
				glCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentHandle));
				glCall(glTexImage2D(GL_TEXTURE_2D, 0, utility_GetInternalFormat(m_Channels), size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
				glCall(glBindTexture(GL_TEXTURE_2D, 0));
				break;
		}
		switch (m_DepthType) {
			case FramebufferDepthType::ABSENT:
				break;
			case FramebufferDepthType::PRESENT:
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilAttachmentHandle));
				glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y));
				glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
				break;
			case FramebufferDepthType::PRESENT_BINDABLE:
				glCall(glBindTexture(GL_TEXTURE_2D, m_DepthStencilAttachmentHandle));
				glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
				glCall(glBindTexture(GL_TEXTURE_2D, 0));
				break;
		}
	}

}