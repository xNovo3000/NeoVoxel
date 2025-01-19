#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

#include <optional>
#include <string>
#include <vector>

namespace NeoVoxel {

	namespace Asset {

		using BinaryFile = std::vector<uint8_t>;
		using StringFile = std::string;

		struct Audio {
			std::vector<int16_t> m_Data;
			int32_t m_Frequency;
		};

		struct Image {
			std::vector<uint8_t> m_Data;
			glm::ivec2 m_Size;
		};

	}

	class AssetLoader {

	public:
		AssetLoader();
		~AssetLoader() = default;

		AssetLoader(const AssetLoader&) = delete;
		AssetLoader& operator=(const AssetLoader&) = delete;

		AssetLoader(AssetLoader&&) noexcept = default;
		AssetLoader& operator=(AssetLoader&&) noexcept = default;

		std::optional<Asset::BinaryFile> loadBinaryFile(const std::string& path) const;
		std::optional<Asset::StringFile> loadStringFile(const std::string& path) const;
		std::optional<Asset::Audio> loadAudio(const std::string& path) const;
		std::optional<Asset::Image> loadImage(const std::string& path) const;

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

}