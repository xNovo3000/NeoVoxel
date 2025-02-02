#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>

namespace NeoVoxel {

	class QuadBatch {

	public:
		QuadBatch();

		void updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data);
		void updateIndices(const std::vector<uint32_t>& data);

		const ArrayBufferRef& getArrayBuffer() const noexcept { return m_ArrayBuffer; }

	private:
		ArrayBufferRef m_ArrayBuffer;

	};

}