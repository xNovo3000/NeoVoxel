#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/Texture.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	struct FramebufferSpec {
		TextureChannels m_Channels;
		glm::ivec2 m_Size;
	};

	class Framebuffer {

	public:
		Framebuffer(const Texture2DRef& colorAttachment);
		virtual ~Framebuffer() = default;

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer& operator=(Framebuffer&&) noexcept = default;

		virtual void bind();

		virtual void setSize(const glm::ivec2& size);

		const Texture2DRef& getColorAttachment() const noexcept { return m_ColorAttachment; }

	protected:
		Texture2DRef m_ColorAttachment;

	};

	using FramebufferRef = Ref<Framebuffer>;

}