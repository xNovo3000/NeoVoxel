#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

#include <map>
#include <utility>
#include <vector>

namespace NeoVoxel {

	enum class TextureChannels : uint8_t { RED_8, RGB_8, RGBA_8, RGB_16, RGBA_16, SRGB_8, SRGBA_8 };
	enum class TextureMipmapGeneration : uint8_t { ENABLED, DISABLED };

	enum class TextureParamsName : uint8_t { WRAP_S, WRAP_T, FILTER_MIN, FILTER_MAG };
	enum class TextureParamsValue : uint8_t { WRAP_CLAMP_TO_EDGE, WRAP_REPEAT, FILTER_NEAREST, FILTER_LINEAR };

	struct Texture2DSpec {
		TextureChannels m_Channels;
		TextureMipmapGeneration m_MipmapGeneration;
		std::map<TextureParamsName, TextureParamsValue> m_Parameters;
	};

	class Texture2D {

	public:
		Texture2D() = default;
		virtual ~Texture2D() = default;

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		Texture2D(Texture2D&&) noexcept = default;
		Texture2D& operator=(Texture2D&&) noexcept = default;

		virtual void bind(uint32_t slot = 0);

		virtual void update(const glm::ivec2& size);
		virtual void update(const glm::ivec2& size, const std::vector<uint8_t>& data);
		virtual void subUpdate(const glm::ivec2& size, const glm::ivec2& offset, const std::vector<uint8_t>& data);

	};

	using Texture2DRef = Ref<Texture2D>;

}