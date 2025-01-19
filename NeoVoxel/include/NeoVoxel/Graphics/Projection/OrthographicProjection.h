#pragma once

#include <NeoVoxel/Graphics/Projection.h>

namespace NeoVoxel {

	class OrthographicProjection : public Projection {

	public:
		OrthographicProjection();
		OrthographicProjection(float zoom);
		~OrthographicProjection() = default;

		OrthographicProjection(const OrthographicProjection&) = default;
		OrthographicProjection& operator=(const OrthographicProjection&) = default;

		OrthographicProjection(OrthographicProjection&&) noexcept = default;
		OrthographicProjection& operator=(OrthographicProjection&&) noexcept = default;

		void setZoom(float zoom);

		virtual glm::mat4 getProjectionMatrix(float aspectRatio) const override;
		float getZoom() const noexcept { return m_Zoom; }

	private:
		float m_Zoom;

	};

}