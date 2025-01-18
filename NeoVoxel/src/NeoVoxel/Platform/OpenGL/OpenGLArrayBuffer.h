#pragma once

#include <NeoVoxel/Graphics/ArrayBuffer.h>

#include <glad/gl.h>

#include <array>

namespace NeoVoxel {

	constexpr size_t MAX_BUFFERS = 8;

	class OpenGLArrayBuffer : public ArrayBuffer {

	public:
		OpenGLArrayBuffer(const ArrayBufferSpec& spec);
		virtual ~OpenGLArrayBuffer();

		OpenGLArrayBuffer(const OpenGLArrayBuffer&) = delete;
		OpenGLArrayBuffer& operator=(const OpenGLArrayBuffer&) = delete;

		OpenGLArrayBuffer(OpenGLArrayBuffer&& other) noexcept;
		OpenGLArrayBuffer& operator=(OpenGLArrayBuffer&& other) noexcept;

		virtual void render() const override;

		virtual void setVertexBufferData(uint32_t index, const std::vector<float>& data) override;
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec2>& data) override;
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec3>& data) override;
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec4>& data) override;
		virtual void setVertexBufferData(uint32_t index, const std::vector<std::pair<glm::vec2, glm::vec2>>& data) override;
		virtual void setVertexBufferData(uint32_t index, const std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec3>>& data) override;

		virtual void setElementBufferData(const std::vector<uint32_t>& data) override;

	private:
		GLuint m_VaoHandle;
		GLuint m_EboHandle;
		std::array<GLuint, MAX_BUFFERS> m_VboHandles;
		uint32_t m_DrawType : 1;  // ArrayBufferDrawType m_DrawType;
		uint32_t m_NumberOfVbos : 31;
		uint32_t m_NumberOfVertices;

	};

}