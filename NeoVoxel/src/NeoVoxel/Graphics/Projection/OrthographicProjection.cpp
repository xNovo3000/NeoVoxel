#include <PCH.h>
#include <NeoVoxel/Graphics/Projection/OrthographicProjection.h>

#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	OrthographicProjection::OrthographicProjection() : OrthographicProjection(1.0F) {}
	OrthographicProjection::OrthographicProjection(float zoom) : m_Zoom(zoom) {}

	void OrthographicProjection::setZoom(float zoom) {
		m_Zoom = zoom;
		if (m_Zoom <= 0.0F) {
			m_Zoom = glm::epsilon<float>();
		}
	}

	glm::mat4 OrthographicProjection::getProjectionMatrix(float aspectRatio) const {
		auto vertical = 1 / m_Zoom;
		auto horizontal = vertical * aspectRatio;
		return glm::ortho(-horizontal, horizontal, -vertical, vertical);
	}

}