#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>
#include <NeoVoxel/Graphics/Framebuffer.h>
#include <NeoVoxel/Graphics/Shader.h>
#include <NeoVoxel/Graphics/Texture.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG
	constexpr glm::vec4 DEFAULT_CLEAR_COLOR = glm::vec4(1.0F, 0.0F, 1.0F, 0.0F);
	constexpr float DEFAULT_CLEAR_DEPTH = 1.0F;
#else
	constexpr glm::vec4 DEFAULT_CLEAR_COLOR = glm::vec4(0.0F, 0.0F, 0.0F, 0.0F);
	constexpr float DEFAULT_CLEAR_DEPTH = 1.0F;
#endif

	class GraphicsApi {

	public:
		GraphicsApi();
		GraphicsApi(const char* name);
		virtual ~GraphicsApi() = default;

		GraphicsApi(const GraphicsApi&) = delete;
		GraphicsApi& operator=(const GraphicsApi&) = delete;

		GraphicsApi(GraphicsApi&&) noexcept = default;
		GraphicsApi& operator=(GraphicsApi&&) noexcept = default;

		virtual void clearColor(const glm::vec4& color = DEFAULT_CLEAR_COLOR);
		virtual void clearDepth(float depth = DEFAULT_CLEAR_DEPTH);

		virtual void setViewport(const glm::ivec2& viewport);

		virtual void unbindFramebuffer();

		virtual ArrayBufferRef createArrayBuffer(const ArrayBufferSpec& spec);
		virtual FramebufferRef createFramebuffer(const FramebufferSpec& spec);
		virtual ShaderRef createShader(const ShaderSpec& spec);
		virtual Texture2DRef createTexture2D(const Texture2DSpec& spec);

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

	using GraphicsApiRef = Ref<GraphicsApi>;

}