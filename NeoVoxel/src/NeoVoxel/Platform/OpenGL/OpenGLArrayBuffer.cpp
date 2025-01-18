#include <PCH.h>
#include "OpenGLArrayBuffer.h"
#include "OpenGLErrorManagement.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {

	/* Utility */

	static inline GLenum utility_GetDrawType(uint32_t drawType) {
		switch (drawType) {
			case 0: return GL_STATIC_DRAW;
			case 1: return GL_DYNAMIC_DRAW;
		}
		return GL_STATIC_DRAW;
	}

	/* OpenGLArrayBuffer */

	OpenGLArrayBuffer::OpenGLArrayBuffer(const ArrayBufferSpec& spec) : ArrayBuffer(),
		m_VaoHandle(OPENGL_INVALID_HANDLE), m_EboHandle(OPENGL_INVALID_HANDLE), m_VboHandles(),
		m_NumberOfVbos(0), m_DrawType(static_cast<uint32_t>(spec.m_DrawType)), m_NumberOfVertices(0)
	{
		NV_PROFILE;
	}

	OpenGLArrayBuffer::~OpenGLArrayBuffer() {
		if (m_VaoHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
		}
	}

	OpenGLArrayBuffer::OpenGLArrayBuffer(OpenGLArrayBuffer&& other) noexcept : ArrayBuffer(std::move(other)),
		m_VaoHandle(other.m_VaoHandle), m_EboHandle(other.m_EboHandle), m_VboHandles(std::move(other.m_VboHandles)),
		m_NumberOfVbos(other.m_NumberOfVbos), m_DrawType(other.m_DrawType), m_NumberOfVertices(other.m_NumberOfVertices)
	{
		other.m_VaoHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLArrayBuffer& OpenGLArrayBuffer::operator=(OpenGLArrayBuffer&& other) noexcept {
		m_VaoHandle = other.m_VaoHandle;
		m_EboHandle = other.m_EboHandle;
		m_VboHandles = std::move(other.m_VboHandles);
		m_NumberOfVbos = other.m_NumberOfVbos;
		m_DrawType = other.m_DrawType;
		m_NumberOfVertices = other.m_NumberOfVertices;
		other.m_VaoHandle = OPENGL_INVALID_HANDLE;
		return *this;
	}

	void OpenGLArrayBuffer::render() const {
		NV_PROFILE;
		glCall(glBindVertexArray(m_VaoHandle));
		glCall(glDrawElements(GL_TRIANGLES, m_NumberOfVertices, utility_GetDrawType(m_DrawType), nullptr));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<float>& data)
	{
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec2>& data)
	{
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec3>& data)
	{
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec4>& data)
	{
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::pair<glm::vec2, glm::vec2>>& data)
	{
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec3>>& data)
	{
	}

	void OpenGLArrayBuffer::setElementBufferData(const std::vector<uint32_t>& data)
	{
	}

}