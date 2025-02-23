#include <PCH.h>
#include <NeoVoxel/Graphics/Mesh/DefaultMesh.h>
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	MeshRef DefaultMesh::create() {
		return std::make_shared<DefaultMesh>();
	}

	DefaultMesh::DefaultMesh() : Mesh() {
		auto& graphicsApi = NV_GET_GRAPHICS_API;
		ArrayBufferSpec spec{
			{ ArrayBufferElement::PACKED_POSITION_UV_NORMAL_3D },
			ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = graphicsApi.createArrayBuffer(spec);
	}

	void DefaultMesh::updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data) {
		m_ArrayBuffer->setVertexBufferData(0, data);
	}

	void DefaultMesh::updateIndices(const std::vector<uint32_t>& data) {
		m_ArrayBuffer->setElementBufferData(data);
	}

}