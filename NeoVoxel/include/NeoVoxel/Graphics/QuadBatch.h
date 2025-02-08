#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>

namespace NeoVoxel {

	class QuadBatch {

	public:
		QuadBatch() = default;
		virtual ~QuadBatch() = default;

		QuadBatch(const QuadBatch&) = delete;
		QuadBatch& operator=(const QuadBatch&) = delete;

		QuadBatch(QuadBatch&&) noexcept = default;
		QuadBatch& operator=(QuadBatch&&) noexcept = default;

		virtual void updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data);
		virtual void updateIndices(const std::vector<uint32_t>& data);

		const ArrayBufferRef& getArrayBuffer() const noexcept { return m_ArrayBuffer; }

	protected:
		ArrayBufferRef m_ArrayBuffer;

	};

	using QuadBatchRef = Ref<QuadBatch>;

}