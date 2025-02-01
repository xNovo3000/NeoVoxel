#include <PCH.h>
#include <NeoVoxel/Graphics/Framebuffer.h>

namespace NeoVoxel {

	void Framebuffer::bind(FramebufferBinding binding) {}
	void Framebuffer::bindColorAttachment(uint32_t slot) {}
	void Framebuffer::bindDepthAttachment(uint32_t slot) {}

	void Framebuffer::setSize(const glm::ivec2& size) {}

}