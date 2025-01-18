#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

#include <optional>
#include <string>
#include <vector>

namespace NeoVoxel {

	struct ShaderSpec {
		std::string m_VertexShader;
		std::optional<std::string> m_GeometryShader;
		std::string m_FragmentShader;
	};

	class Shader {

	public:
		virtual ~Shader() = default;

		virtual void activate();

		virtual void setUniform(uint32_t location, int32_t value);
		virtual void setUniform(uint32_t location, float value);
		virtual void setUniform(uint32_t location, const glm::vec2& value);
		virtual void setUniform(uint32_t location, const glm::vec3& value);
		virtual void setUniform(uint32_t location, const glm::mat3& value);
		virtual void setUniform(uint32_t location, const glm::mat4& value);

	};

	using ShaderRef = Ref<Shader>;

}