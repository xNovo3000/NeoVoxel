#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	enum class FramebufferBinding : uint8_t { READ, WRITE, ALL };
	enum class FramebufferColorChannels : uint8_t { RGB_8, RGBA_8, RGB_16, RGBA_16 };
	enum class FramebufferColorType : uint8_t { NONE, BUFFER, TEXTURE, BUFFER_MSAA };
	enum class FramebufferDepthType : uint8_t { NONE, BUFFER, TEXTURE, BUFFER_MSAA };

	struct FramebufferSpec {
		FramebufferColorChannels m_Channels;
		FramebufferColorType m_ColorType;
		FramebufferDepthType m_DepthType;
		glm::ivec2 m_Size;
	};

	class Framebuffer {

	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer& operator=(Framebuffer&&) noexcept = default;

		virtual void bind(FramebufferBinding binding = FramebufferBinding::ALL);
		virtual void bindColorAttachment(uint32_t slot = 0);
		virtual void bindDepthAttachment(uint32_t slot = 0);

		virtual void setSize(const glm::ivec2& size);

	};

	using FramebufferRef = Ref<Framebuffer>;

}