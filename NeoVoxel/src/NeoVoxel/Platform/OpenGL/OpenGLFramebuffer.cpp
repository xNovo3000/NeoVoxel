#include <PCH.h>
#include "OpenGLFramebuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLErrorManagement.h"
#include "OpenGLMemoryProfile.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {

	OpenGLFramebuffer::OpenGLFramebuffer(const Texture2DRef& colorAttachment, const FramebufferSpec& spec) :
		Framebuffer(colorAttachment), m_FramebufferHandle(OPENGL_INVALID_HANDLE),
		m_DepthStencilRenderbufferHandle(OPENGL_INVALID_HANDLE)
	{
		NV_PROFILE;
		NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, spec.m_Size.x * spec.m_Size.y * 4);
		// Depth-stencil renderbuffer
		glCall(glGenRenderbuffers(1, &m_DepthStencilRenderbufferHandle));
		glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRenderbufferHandle));
		glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, spec.m_Size.x, spec.m_Size.y));
		glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		// Framebuffer
		glCall(glGenFramebuffers(1, &m_FramebufferHandle));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reinterpret_cast<OpenGLTexture2D&>(*m_ColorAttachment).getTextureHandle(), 0));
		glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderbufferHandle));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		if (m_FramebufferHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
			NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, 0);
			glCall(glDeleteRenderbuffers(1, &m_DepthStencilRenderbufferHandle));
			glCall(glDeleteFramebuffers(1, &m_FramebufferHandle));
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(OpenGLFramebuffer&& other) noexcept :
		Framebuffer(std::move(other)), m_FramebufferHandle(other.m_FramebufferHandle),
		m_DepthStencilRenderbufferHandle(other.m_DepthStencilRenderbufferHandle)
	{
		other.m_FramebufferHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLFramebuffer& OpenGLFramebuffer::operator=(OpenGLFramebuffer&& other) noexcept {
		Framebuffer::operator=(std::move(other));
		m_FramebufferHandle = other.m_FramebufferHandle;
		m_DepthStencilRenderbufferHandle = other.m_DepthStencilRenderbufferHandle;
		other.m_FramebufferHandle = OPENGL_INVALID_HANDLE;
		return *this;
	}

	void OpenGLFramebuffer::bind() {
		NV_PROFILE;
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
	}

	void OpenGLFramebuffer::setSize(const glm::ivec2& size) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, size.x * size.y * 4);
		m_ColorAttachment->update(size);
		glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRenderbufferHandle));
		glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y));
		glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

}