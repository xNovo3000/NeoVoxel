#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>

namespace NeoVoxel {

	class Mesh {

	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&&) noexcept = default;
		Mesh& operator=(Mesh&&) noexcept = default;

		virtual void updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data);
		virtual void updateIndices(const std::vector<uint32_t>& data);

		const ArrayBufferRef& getArrayBuffer() const noexcept { return m_ArrayBuffer; }

	protected:
		ArrayBufferRef m_ArrayBuffer;

	};

	using MeshRef = Ref<Mesh>;

}