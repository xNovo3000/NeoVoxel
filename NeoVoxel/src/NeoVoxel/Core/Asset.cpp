#include <PCH.h>
#include <NeoVoxel/Core/Asset.h>
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_vorbis.c>  // WARNING: Make sure to include this file only once

namespace NeoVoxel {

	AssetLoader::AssetLoader() : m_Name("AssetLoader") {
		stbi_set_flip_vertically_on_load(true);
	}

	std::optional<Asset::BinaryFile> AssetLoader::loadBinaryFile(const std::string& path) const {
		NV_PROFILE;
		// Check if file exists
		if (!std::filesystem::exists(path)) {
			NV_WARN("{}: binary file '{}' does not exists", m_Name, path);
			return std::nullopt;
		}
		// Load file
		std::fstream inputStream(path, std::ios::in | std::ios::binary);
		return Asset::BinaryFile{ std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>() };
	}

	std::optional<Asset::StringFile> AssetLoader::loadStringFile(const std::string& path) const {
		NV_PROFILE;
		// Check if file exists
		if (!std::filesystem::exists(path)) {
			NV_WARN("{}: string file '{}' does not exists", m_Name, path);
			return std::nullopt;
		}
		// Load file
		std::fstream inputStream(path, std::ios::in | std::ios::binary);
		return Asset::StringFile{ std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>() };
	}

	std::optional<Asset::Audio> AssetLoader::loadAudio(const std::string& path) const {
		NV_PROFILE;
		// Check if file exists
		if (!std::filesystem::exists(path)) {
			NV_WARN("{}: audio file '{}' does not exists", m_Name, path);
			return std::nullopt;
		}
		// Create containers
		int32_t audioFrequency = 0;
		int32_t audioChannels = 0;
		int16_t* audioData = nullptr;
		int32_t audioDataSize = -1;
		// Load audio
		audioDataSize = stb_vorbis_decode_filename(path.c_str(), &audioChannels, &audioFrequency, &audioData);
		// Check if audio is ok
		if (audioDataSize == -1) {
			NV_ERROR("{}: failed to load audio file '{}'. Decoding error", m_Name, path);
			return std::nullopt;
		}
		if (audioChannels != 1) {
			NV_ERROR("{}: failed to load audio file '{}'. Only 1-channel audios are supported", m_Name, path);
			return std::nullopt;
		}
		// Create the final container
		auto data = std::vector<int16_t>(audioData, audioData + audioDataSize);
		// Free stb allocated memory
		free(audioData);
		// Return the asset
		return Asset::Audio{ data, audioFrequency };
	}

	std::optional<Asset::Image> AssetLoader::loadImage(const std::string& path) const {
		NV_PROFILE;
		// Check if file exists
		if (!std::filesystem::exists(path)) {
			NV_WARN("{}: image file '{}' does not exists", m_Name, path);
			return std::nullopt;
		}
		// Create metadata containers
		glm::ivec2 size = {};
		int32_t channels;
		// stb: load image
		auto imageData = stbi_load(path.c_str(), &size.x, &size.y, &channels, STBI_rgb_alpha);
		if (imageData == nullptr) {
			NV_ERROR("{}: cannot load image '{}'. Decoding error", m_Name, path);
			return std::nullopt;
		}
		// Copy data inside an STL container
		std::vector<uint8_t> data(imageData, imageData + size.x * size.y * channels);
		// Clear stb pointer
		stbi_image_free(imageData);
		// Return the image struct
		return Asset::Image{ data, size };
	}

}