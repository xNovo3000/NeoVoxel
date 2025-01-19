#include <PCH.h>
#include <NeoVoxel/Graphics/Texture.h>

namespace NeoVoxel {

	void Texture2D::bind(uint32_t slot) {}

	void Texture2D::update(const glm::ivec2& size) {}
	void Texture2D::update(const glm::ivec2& size, const std::vector<uint8_t>& data) {}
	void Texture2D::subUpdate(const glm::ivec2& size, const glm::ivec2& offset, const std::vector<uint8_t>& data) {}

}