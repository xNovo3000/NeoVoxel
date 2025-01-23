#include <PCH.h>
#include <NeoVoxel/Graphics/GraphicsApi.h>

namespace NeoVoxel {

	GraphicsApi::GraphicsApi() : GraphicsApi("GraphicsApi") {}
	GraphicsApi::GraphicsApi(const char* name) : m_Name(name) {}

	void GraphicsApi::clearColor(const glm::vec4& color) {}
	void GraphicsApi::clearDepth(float depth) {}

	void GraphicsApi::setViewport(const glm::ivec2& viewport) {}

	void GraphicsApi::unbindFramebuffer() {}

	ArrayBufferRef GraphicsApi::createArrayBuffer(const ArrayBufferSpec& spec) {
		return std::make_shared<ArrayBuffer>();
	}

	FramebufferRef GraphicsApi::createFramebuffer(const FramebufferSpec& spec) {
		Texture2DSpec textureSpec{
			spec.m_ColorSpace,
			TextureMipmapGeneration::DISABLED,
			{
				{ TextureParamsName::FILTER_MAG, TextureParamsValue::FILTER_LINEAR },
				{ TextureParamsName::FILTER_MIN, TextureParamsValue::FILTER_NEAREST },
				{ TextureParamsName::WRAP_S, TextureParamsValue::WRAP_CLAMP_TO_EDGE },
				{ TextureParamsName::WRAP_T, TextureParamsValue::WRAP_CLAMP_TO_EDGE },
			}
		};
		auto textureRef = createTexture2D(textureSpec);
		return std::make_shared<Framebuffer>(textureRef);
	}

	ShaderRef GraphicsApi::createShader(const ShaderSpec& spec) {
		return std::make_shared<Shader>();
	}

	Texture2DRef GraphicsApi::createTexture2D(const Texture2DSpec& spec) {
		return std::make_shared<Texture2D>();
	}

}