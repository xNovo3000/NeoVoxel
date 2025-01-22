#pragma once

#include <NeoVoxel/Graphics/Framebuffer.h>

#include <glad/gl.h>

namespace NeoVoxel {

	class OpenGLFramebuffer : public Framebuffer {

	public:
		OpenGLFramebuffer(const Texture2DRef& colorAttachment, const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		OpenGLFramebuffer(const OpenGLFramebuffer&) = delete;
		OpenGLFramebuffer& operator=(const OpenGLFramebuffer&) = delete;

		OpenGLFramebuffer(OpenGLFramebuffer&& other) noexcept;
		OpenGLFramebuffer& operator=(OpenGLFramebuffer&& other) noexcept;

		virtual void bind() override;

		virtual void setSize(const glm::ivec2& size) override;

	private:
		GLuint m_FramebufferHandle;
		GLuint m_DepthStencilRenderbufferHandle;

	};

}