#pragma once

#include <NeoVoxel/Graphics/QuadBatch.h>

namespace NeoVoxel {

	class DefaultQuadBatch : public QuadBatch {

	public:
		static QuadBatchRef create();

	public:
		DefaultQuadBatch();
		virtual ~DefaultQuadBatch() = default;

		DefaultQuadBatch(const DefaultQuadBatch&) = delete;
		DefaultQuadBatch& operator=(const DefaultQuadBatch&) = delete;

		DefaultQuadBatch(DefaultQuadBatch&&) noexcept = default;
		DefaultQuadBatch& operator=(DefaultQuadBatch&&) noexcept = default;

		virtual void updateVertices(const std::vector<std::pair<glm::vec2, glm::vec2>>& data) override;
		virtual void updateIndices(const std::vector<uint32_t>& data) override;

	};

}