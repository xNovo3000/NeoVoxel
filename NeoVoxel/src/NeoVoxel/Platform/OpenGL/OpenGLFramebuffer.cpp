#include <PCH.h>
#include "OpenGLFramebuffer.h"
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
		
	}

	OpenGLFramebuffer& OpenGLFramebuffer::operator=(OpenGLFramebuffer&& other) noexcept {
		Framebuffer::operator=(std::move(other));
		return *this;
	}

	void OpenGLFramebuffer::bind() {
		NV_PROFILE;
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
	}

	void OpenGLFramebuffer::setSize(const glm::ivec2& size) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_FRAMEBUFFER(m_FramebufferHandle, size.x * size.y * 4);
	}

}