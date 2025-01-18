#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

#include <tuple>
#include <utility>
#include <vector>

namespace NeoVoxel {

	enum class ArrayBufferDrawType { STATIC, DYNAMIC };
	enum class ArrayBufferElement { PACKED_POSITION_UV_NORMAL_3D, PACKED_POSITION_UV_2D, VEC4, VEC3, VEC2, FLOAT };

	struct ArrayBufferSpec {
		std::vector<ArrayBufferElement> m_Elements;
		ArrayBufferDrawType m_DrawType;
	};

	class ArrayBuffer {

	public:
		ArrayBuffer() = default;
		virtual ~ArrayBuffer() = default;

		ArrayBuffer(const ArrayBuffer&) = delete;
		ArrayBuffer& operator=(const ArrayBuffer&) = delete;

		ArrayBuffer(ArrayBuffer&&) noexcept = default;
		ArrayBuffer& operator=(ArrayBuffer&&) noexcept = default;

		virtual void render() const;

		virtual void setVertexBufferData(uint32_t index, const std::vector<float>& data);
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec2>& data);
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec3>& data);
		virtual void setVertexBufferData(uint32_t index, const std::vector<glm::vec4>& data);
		virtual void setVertexBufferData(uint32_t index, const std::vector<std::pair<glm::vec2, glm::vec2>>& data);
		virtual void setVertexBufferData(uint32_t index, const std::vector<std::tuple<glm::vec3, glm::vec2, glm::vec3>>& data);

		virtual void setElementBufferData(const std::vector<uint32_t>& data);

	};

	using ArrayBufferRef = Ref<ArrayBuffer>;

}