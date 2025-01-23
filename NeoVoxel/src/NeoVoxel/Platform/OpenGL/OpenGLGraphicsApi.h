#pragma once

#include <NeoVoxel/Graphics/GraphicsApi.h>

namespace NeoVoxel {

	using OpenGLLoaderPtr = void (*(*)(const char*))(void);

	class OpenGLGraphicsApi : public GraphicsApi {

	public:
		OpenGLGraphicsApi(OpenGLLoaderPtr loaderFunction);
		virtual ~OpenGLGraphicsApi() = default;

		OpenGLGraphicsApi(const OpenGLGraphicsApi&) = delete;
		OpenGLGraphicsApi& operator=(const OpenGLGraphicsApi&) = delete;

		OpenGLGraphicsApi(OpenGLGraphicsApi&&) noexcept = default;
		OpenGLGraphicsApi& operator=(OpenGLGraphicsApi&&) noexcept = default;

		virtual void clearColor(const glm::vec4& color) override;
		virtual void clearDepth(float depth) override;

		virtual void setViewport(const glm::ivec2& viewport) override;

		virtual void unbindFramebuffer() override;

		virtual ArrayBufferRef createArrayBuffer(const ArrayBufferSpec& spec) override;
		virtual FramebufferRef createFramebuffer(const FramebufferSpec& spec) override;
		virtual ShaderRef createShader(const ShaderSpec& spec) override;
		virtual Texture2DRef createTexture2D(const Texture2DSpec& spec) override;

	};

}