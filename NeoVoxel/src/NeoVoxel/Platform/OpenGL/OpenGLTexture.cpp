#include <PCH.h>
#include "OpenGLTexture.h"
#include "OpenGLErrorManagement.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {



	OpenGLTexture2D::OpenGLTexture2D()
	{
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
	}

	OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& other) noexcept
	{
	}

	OpenGLTexture2D& OpenGLTexture2D::operator=(OpenGLTexture2D&& other) noexcept
	{
		// TODO: inserire l'istruzione return qui
	}

	void OpenGLTexture2D::bind(uint32_t slot)
	{
	}

	void OpenGLTexture2D::update(const glm::ivec2& size)
	{
	}

	void OpenGLTexture2D::update(const glm::ivec2& size, const std::vector<uint8_t>& data)
	{
	}

	void OpenGLTexture2D::subUpdate(const glm::ivec2& size, const glm::ivec2& offset, const std::vector<uint8_t>& data)
	{
	}

}