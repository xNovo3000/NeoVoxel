#include <PCH.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>

namespace NeoVoxel {

	void ArrayBuffer::render() const {}

	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<float>& data) {}
	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec2>& data) {}
	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec3>& data) {}
	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<glm::vec4>& data) {}
	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::pair<glm::vec2, glm::vec2>>& data) {}
	void ArrayBuffer::setVertexBufferData(uint32_t index, const std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec3>>& data) {}

	void ArrayBuffer::setElementBufferData(const std::vector<uint32_t>& data) {}

}