#include <PCH.h>
#include <NeoVoxel/Graphics/Framebuffer.h>

namespace NeoVoxel {

	/* Framebuffer */

	void Framebuffer::bind(FramebufferBinding binding) {}
	void Framebuffer::bindColorAttachment(uint32_t slot) {}
	void Framebuffer::bindDepthAttachment(uint32_t slot) {}

	void Framebuffer::setSize(const glm::ivec2& size) {}

	/* FramebufferMultisample */

	void FramebufferMultisample::bind(FramebufferBinding binding) {}

	void FramebufferMultisample::setSize(const glm::ivec2& size) {}

}