#include <PCH.h>
#include <NeoVoxel/Graphics/GraphicsApi.h>

namespace NeoVoxel {

	GraphicsApi::GraphicsApi() : GraphicsApi("GraphicsApi") {}
	GraphicsApi::GraphicsApi(const char* name) : m_Name(name) {}

	void GraphicsApi::clearColor(const glm::vec4& color) {}
	void GraphicsApi::clearDepth(float depth) {}

	void GraphicsApi::setViewport(const glm::ivec2& viewport) {}

}