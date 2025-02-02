#include <PCH.h>
#include <NeoVoxel/Graphics/GraphicsApi.h>

namespace NeoVoxel {

	GraphicsApi::GraphicsApi() : GraphicsApi("GraphicsApi") {}
	GraphicsApi::GraphicsApi(const char* name) : m_Name(name) {}

	void GraphicsApi::clearColor(const glm::vec4& color) {}
	void GraphicsApi::clearDepth(float depth) {}

	void GraphicsApi::setViewport(const glm::ivec2& viewport) {}

	void GraphicsApi::copyFramebufferColorData(const FramebufferRef& source, const FramebufferRef& destination, const glm::ivec2& size) {}
	void GraphicsApi::unbindFramebuffer() {}

	ArrayBufferRef GraphicsApi::createArrayBuffer(const ArrayBufferSpec& spec) {
		return std::make_shared<ArrayBuffer>();
	}

	FramebufferRef GraphicsApi::createFramebuffer(const FramebufferSpec& spec) {
		return std::make_shared<Framebuffer>();
	}

	ShaderRef GraphicsApi::createShader(const ShaderSpec& spec) {
		return std::make_shared<Shader>();
	}

	Texture2DRef GraphicsApi::createTexture2D(const Texture2DSpec& spec) {
		return std::make_shared<Texture2D>();
	}

}