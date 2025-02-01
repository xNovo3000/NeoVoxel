#pragma once

#include <NeoVoxel/Graphics/Framebuffer.h>

#include <glad/gl.h>

namespace NeoVoxel {

	class OpenGLFramebuffer : public Framebuffer {

	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		OpenGLFramebuffer(const OpenGLFramebuffer&) = delete;
		OpenGLFramebuffer& operator=(const OpenGLFramebuffer&) = delete;

		OpenGLFramebuffer(OpenGLFramebuffer&& other) noexcept;
		OpenGLFramebuffer& operator=(OpenGLFramebuffer&& other) noexcept;

		virtual void bind(FramebufferBinding binding) override;
		virtual void bindColorAttachment(uint32_t slot = 0) override;
		virtual void bindDepthAttachment(uint32_t slot = 0) override;

		virtual void setSize(const glm::ivec2& size) override;

	private:
		GLuint m_FramebufferHandle;
		GLuint m_ColorAttachmentHandle;
		GLuint m_DepthStencilAttachmentHandle;
		FramebufferColorChannels m_Channels;
		FramebufferColorType m_ColorType;
		FramebufferDepthType m_DepthType;

	};

}