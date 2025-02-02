#include <PCH.h>
#include <NeoVoxel/Graphics/Renderer.h>

namespace NeoVoxel {

	Renderer::Renderer(const WindowRef& window, const GraphicsApiRef& graphicsApi) :
		m_Window(window), m_GraphicsApi(graphicsApi)
	{
		
	}

	void Renderer::beginScene(const Camera2D& camera, const Projection& projection) {
		
	}

	void Renderer::submit(const QuadBatch& quadBatch, const MaterialRef& material, const Transform2D& transform) {

	}

	void Renderer::endScene(float exposure, float gamma, const glm::mat3& kernel) {

	}

	void Renderer::setWindowSize(const glm::ivec2& windowSize) {
		// Update FBO sizes
		m_AntialiasingFramebuffer->setSize(windowSize);
		m_PostprocessingFramebuffer->setSize(windowSize);
	}

}