#pragma once

#include <NeoVoxel/Graphics/Texture.h>

#include <glad/gl.h>

namespace NeoVoxel {

	class OpenGLTexture2D : public Texture2D {

	public:
		OpenGLTexture2D(const Texture2DSpec& spec);
		virtual ~OpenGLTexture2D();

		OpenGLTexture2D(const OpenGLTexture2D&) = delete;
		OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;

		OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;
		OpenGLTexture2D& operator=(OpenGLTexture2D&& other) noexcept;

		virtual void bind(uint32_t slot = 0);

		virtual void update(const glm::ivec2& size);
		virtual void update(const glm::ivec2& size, const std::vector<uint8_t>& data);
		virtual void subUpdate(const glm::ivec2& size, const glm::ivec2& offset, const std::vector<uint8_t>& data);

		GLuint getTextureHandle() const noexcept { return m_TextureHandle; }

	private:
		GLuint m_TextureHandle;
		TextureColorSpace m_ColorSpace;
		TextureMipmapGeneration m_GenerateMipmaps;

	};

}