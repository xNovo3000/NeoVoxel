#pragma once

#include <NeoVoxel/Graphics/Projection.h>

namespace NeoVoxel {

	class PerspectiveProjection : public Projection {

	public:
		PerspectiveProjection();
		PerspectiveProjection(float fov);
		~PerspectiveProjection() = default;

		PerspectiveProjection(const PerspectiveProjection&) = default;
		PerspectiveProjection& operator=(const PerspectiveProjection&) = default;

		PerspectiveProjection(PerspectiveProjection&&) noexcept = default;
		PerspectiveProjection& operator=(PerspectiveProjection&&) noexcept = default;

		void setFov(float fov);
		void setFovDegrees(float fovDegrees);

		virtual glm::mat4 getProjectionMatrix(float aspectRatio) const override;
		float getFov() const noexcept { return m_Fov; }

	private:
		float m_Fov;  // In radians

	};

}