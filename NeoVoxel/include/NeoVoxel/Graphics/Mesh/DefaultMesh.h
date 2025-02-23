#pragma once

#include <NeoVoxel/Graphics/Mesh.h>

namespace NeoVoxel {

	class DefaultMesh : public Mesh {

	public:
		static MeshRef create();

	public:
		DefaultMesh();
		virtual ~DefaultMesh() = default;

		DefaultMesh(const DefaultMesh&) = delete;
		DefaultMesh& operator=(const DefaultMesh&) = delete;

		DefaultMesh(DefaultMesh&&) noexcept = default;
		DefaultMesh& operator=(DefaultMesh&&) noexcept = default;

		virtual void updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data) override;
		virtual void updateIndices(const std::vector<uint32_t>& data) override;

	};

}