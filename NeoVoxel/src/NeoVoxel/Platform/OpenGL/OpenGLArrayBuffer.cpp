#include <PCH.h>
#include "OpenGLArrayBuffer.h"
#include "OpenGLErrorManagement.h"
#include "OpenGLMemoryProfile.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

namespace NeoVoxel {

	/* Utility */

	static inline GLenum utility_GetDrawType(ArrayBufferDrawType drawType) {
		switch (drawType) {
			case ArrayBufferDrawType::STATIC: return GL_STATIC_DRAW;
			case ArrayBufferDrawType::DYNAMIC: return GL_DYNAMIC_DRAW;
		}
		return GL_STATIC_DRAW;
	}

	static inline GLsizei utility_GetBufferDimensions(ArrayBufferElement element) {
		switch (element) {
			case ArrayBufferElement::VEC4: return 4;
			case ArrayBufferElement::VEC3: return 3;
			case ArrayBufferElement::VEC2: return 2;
			case ArrayBufferElement::FLOAT: return 1;
		}
		return 0;
	}

	/* OpenGLArrayBuffer */

	OpenGLArrayBuffer::OpenGLArrayBuffer(const ArrayBufferSpec& spec) : ArrayBuffer(),
		m_VaoHandle(OPENGL_INVALID_HANDLE), m_EboHandle(OPENGL_INVALID_HANDLE), m_VboHandles(),
		m_NumberOfVbos(static_cast<uint32_t>(spec.m_Elements.size())),
		m_DrawType(spec.m_DrawType), m_NumberOfVertices(0)
	{
		NV_PROFILE;
		// Out-of-bounds check
		if (spec.m_Elements.size() > MAX_BUFFERS) {
			NV_ERROR("OpenGLArrayBuffer: requested more than {} buffers, only the first 4 will be created", MAX_BUFFERS);
		}
		// Generate handles
		glCall(glGenVertexArrays(1, &m_VaoHandle));
		glCall(glGenBuffers(1, &m_EboHandle));
		glCall(glGenBuffers(m_NumberOfVbos, m_VboHandles.data()));
		// Bind VAO
		glCall(glBindVertexArray(m_VaoHandle));
		// Initialize VBOs
		uint32_t bufferIndex = 0;
		GLuint shaderIndex = 0;
		for (auto element : spec.m_Elements) {
			if (element == ArrayBufferElement::PACKED_POSITION_UV_NORMAL_3D) {
				glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[bufferIndex]));
				glCall(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, utility_GetDrawType(m_DrawType)));
				glCall(glVertexAttribPointer(shaderIndex, 3, GL_FLOAT, GL_FALSE, (GLsizei)(8 * sizeof(float)), nullptr));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
				glCall(glVertexAttribPointer(shaderIndex, 2, GL_FLOAT, GL_FALSE, (GLsizei)(8 * sizeof(float)), (void*)(3 * sizeof(float))));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
				glCall(glVertexAttribPointer(shaderIndex, 3, GL_FLOAT, GL_FALSE, (GLsizei)(8 * sizeof(float)), (void*)(5 * sizeof(float))));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
			}
			else if (element == ArrayBufferElement::PACKED_POSITION_UV_2D) {
				glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[bufferIndex]));
				glCall(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, utility_GetDrawType(m_DrawType)));
				glCall(glVertexAttribPointer(shaderIndex, 2, GL_FLOAT, GL_FALSE, (GLsizei)(4 * sizeof(float)), nullptr));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
				glCall(glVertexAttribPointer(shaderIndex, 2, GL_FLOAT, GL_FALSE, (GLsizei)(4 * sizeof(float)), (void*)(2 * sizeof(float))));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
			}
			else {
				auto dimensions = utility_GetBufferDimensions(element);
				glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[bufferIndex]));
				glCall(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, utility_GetDrawType(m_DrawType)));
				glCall(glVertexAttribPointer(shaderIndex, dimensions, GL_FLOAT, GL_FALSE, (GLsizei)(dimensions * sizeof(float)), nullptr));
				glCall(glEnableVertexAttribArray(shaderIndex));
				shaderIndex += 1;
			}
			bufferIndex += 1;
		}
		// Initialize EBO
		glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EboHandle));
		glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, utility_GetDrawType(m_DrawType)));
	}

	OpenGLArrayBuffer::~OpenGLArrayBuffer() {
		if (m_VaoHandle != OPENGL_INVALID_HANDLE) {
			NV_PROFILE;
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 0, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 1, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 2, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 3, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 4, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 5, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 6, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 7, 0);
			NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 8, 0);
			// Destroy handles
			glCall(glDeleteVertexArrays(1, &m_VaoHandle));
			glCall(glDeleteBuffers(1, &m_EboHandle));
			glCall(glDeleteBuffers(m_NumberOfVbos, m_VboHandles.data()));
		}
	}

	OpenGLArrayBuffer::OpenGLArrayBuffer(OpenGLArrayBuffer&& other) noexcept : ArrayBuffer(std::move(other)),
		m_VaoHandle(other.m_VaoHandle), m_EboHandle(other.m_EboHandle), m_VboHandles(std::move(other.m_VboHandles)),
		m_NumberOfVbos(other.m_NumberOfVbos), m_DrawType(other.m_DrawType), m_NumberOfVertices(other.m_NumberOfVertices)
	{
		other.m_VaoHandle = OPENGL_INVALID_HANDLE;
	}

	OpenGLArrayBuffer& OpenGLArrayBuffer::operator=(OpenGLArrayBuffer&& other) noexcept {
		ArrayBuffer::operator=(std::move(other));
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
		glCall(glDrawElements(GL_TRIANGLES, m_NumberOfVertices, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<float>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(float));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec2>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(glm::vec2));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec3>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(glm::vec3));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec4>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(glm::vec4));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec4), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::pair<glm::vec2, glm::vec2>>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(std::pair<glm::vec2, glm::vec2>));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(std::pair<glm::vec2, glm::vec2>), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec3>>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, index, data.size() * sizeof(std::tuple<glm::vec3, glm::vec2, glm::vec3>));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_VboHandles[index]));
		glCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(std::tuple<glm::vec3, glm::vec2, glm::vec3>), data.data(), utility_GetDrawType(m_DrawType)));
	}

	void OpenGLArrayBuffer::setElementBufferData(const std::vector<uint32_t>& data) {
		NV_PROFILE;
		NV_PROFILE_MEMORY_ARRAY_BUFFER(m_VaoHandle, 8, data.size() * sizeof(uint32_t));
		m_NumberOfVertices = static_cast<uint32_t>(data.size());
		glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EboHandle));
		glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), data.data(), utility_GetDrawType(m_DrawType)));
	}

}