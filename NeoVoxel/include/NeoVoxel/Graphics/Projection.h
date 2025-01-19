#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class Projection {

	public:
		virtual glm::mat4 getProjectionMatrix(float aspectRatio) const = 0;

	};

}