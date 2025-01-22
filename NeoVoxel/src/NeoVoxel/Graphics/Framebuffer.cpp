#include <PCH.h>
#include <NeoVoxel/Graphics/Framebuffer.h>

namespace NeoVoxel {

	Framebuffer::Framebuffer(const Texture2DRef& colorAttachment) : m_ColorAttachment(colorAttachment) {}

	void Framebuffer::bind() {}

	void Framebuffer::setSize(const glm::ivec2& size) {}

}