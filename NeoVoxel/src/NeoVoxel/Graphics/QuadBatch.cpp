#include <PCH.h>
#include <NeoVoxel/Graphics/QuadBatch.h>
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	QuadBatch::QuadBatch() {
		auto& graphicsApi = NV_GET_GRAPHICS_API;
		ArrayBufferSpec spec{
			{ ArrayBufferElement::PACKED_POSITION_UV_2D },
			ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = graphicsApi.createArrayBuffer(spec);
	}

	void QuadBatch::updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data) {
		m_ArrayBuffer->setVertexBufferData(0, data);
	}

	void QuadBatch::updateIndices(const std::vector<uint32_t>& data) {
		m_ArrayBuffer->setElementBufferData(data);
	}

}