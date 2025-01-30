#include <PCH.h>
#include <NeoVoxel/Graphics/Projection/PerspectiveProjection.h>

#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	PerspectiveProjection::PerspectiveProjection() : PerspectiveProjection(glm::radians(70.0F)) {}
	PerspectiveProjection::PerspectiveProjection(float fov) : m_Fov(glm::radians(70.0F)) { setFov(fov); }

	void PerspectiveProjection::setFov(float fov) {
		m_Fov = std::clamp(fov, 0.01F, glm::pi<float>());
	}

	void PerspectiveProjection::setFovDegrees(float fovDegrees) {
		setFov(glm::radians(fovDegrees));
	}

	glm::mat4 PerspectiveProjection::getProjectionMatrix(float aspectRatio) const {
		return glm::perspective(m_Fov, aspectRatio, 0.001F, 1000.0F);
	}

}