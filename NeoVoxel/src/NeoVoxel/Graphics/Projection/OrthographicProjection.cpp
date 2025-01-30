#include <PCH.h>
#include <NeoVoxel/Graphics/Projection/OrthographicProjection.h>

#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	OrthographicProjection::OrthographicProjection() : OrthographicProjection(1.0F) {}
	OrthographicProjection::OrthographicProjection(float zoom) : m_Zoom(1.0F) { setZoom(zoom); }

	void OrthographicProjection::setZoom(float zoom) {
		m_Zoom = std::clamp(zoom, 0.01F, 100.0F);
	}

	glm::mat4 OrthographicProjection::getProjectionMatrix(float aspectRatio) const {
		auto vertical = 1 / m_Zoom;
		auto horizontal = vertical * aspectRatio;
		return glm::ortho(-horizontal, horizontal, -vertical, vertical);
	}

}