#include <PCH.h>
#include <NeoVoxel/Graphics/Renderer.h>

namespace NeoVoxel {

	Renderer::Renderer(const WindowRef& window, const GraphicsApiRef& graphicsApi, const AssetLoader& assetLoader) :
		m_Window(window), m_GraphicsApi(graphicsApi)
	{
		// Antialiasing
		FramebufferSpec antialiasingFramebufferSpec{
			FramebufferColorChannels::RGBA_16,
			FramebufferColorType::BUFFER_MSAA,
			FramebufferDepthType::BUFFER_MSAA,
			m_Window->getSize()
		};
		m_AntialiasingFramebuffer = m_GraphicsApi->createFramebuffer(antialiasingFramebufferSpec);
		// Postprocessing
		FramebufferSpec postprocessingFramebufferSpec{
			FramebufferColorChannels::RGBA_16,
			FramebufferColorType::TEXTURE,
			FramebufferDepthType::NONE,
			m_Window->getSize()
		};
		m_PostprocessingFramebuffer = m_GraphicsApi->createFramebuffer(postprocessingFramebufferSpec);
		ArrayBufferSpec postprocessingBufferSpec{
			{ ArrayBufferElement::PACKED_POSITION_UV_2D },
			ArrayBufferDrawType::STATIC
		};
		std::vector<std::pair<glm::vec2, glm::vec2>> postprocessingElements = {
			{ { -1.0F, -1.0F }, { 0.0F, 0.0F } },
			{ {  1.0F, -1.0F }, { 1.0F, 0.0F } },
			{ { -1.0F,  1.0F }, { 0.0F, 1.0F } },
			{ {  1.0F,  1.0F }, { 1.0F, 1.0F } }
		};
		std::vector<uint32_t> postprocessingIndices = { 0, 1, 2, 1, 3, 2 };
		m_PostprocessingBuffer = m_GraphicsApi->createArrayBuffer(postprocessingBufferSpec);
		m_PostprocessingBuffer->setVertexBufferData(0, postprocessingElements);
		m_PostprocessingBuffer->setElementBufferData(postprocessingIndices);
		auto maybePostprocessingVertexShader = assetLoader.loadStringFile("asset/shader/postprocessing.vert");
		auto maybePostprocessingFragmentShader = assetLoader.loadStringFile("asset/shader/postprocessing.frag");
		ShaderSpec postprocessingShaderSpec{
			*maybePostprocessingVertexShader, std::nullopt, *maybePostprocessingFragmentShader
		};
		m_PostprocessingShader = m_GraphicsApi->createShader(postprocessingShaderSpec);
	}

	void Renderer::beginScene(const Camera2D& camera, const Projection& projection) {
		// Get window info
		auto windowSize = m_Window->getSize();
		float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		// Calculate basic matrices
		auto projectionMatrix = projection.getProjectionMatrix(aspectRatio);
		auto viewMatrix = camera.getViewMatrix();
		m_ProjectionViewMatrix = projectionMatrix * viewMatrix;
		// Calculare kernel offset
		m_KernelOffset = { 1.0F / static_cast<float>(windowSize.x), 1.0F / static_cast<float>(windowSize.y) };
		// Enable antialiasing framebuffer and clear it
		m_AntialiasingFramebuffer->bind();
		m_GraphicsApi->clearColor();
		m_GraphicsApi->clearDepth();
	}

	void Renderer::submit(const QuadBatchRef& quadBatch, const MaterialRef& material, const Transform2D& transform) {
		// Calculare model matrix
		auto modelMatrix = transform.getModelMatrix();
		// Enable material and set model matrix data
		material->bind();
		material->getShader()->setUniform("u_ProjectionViewMatrix", m_ProjectionViewMatrix);
		material->getShader()->setUniform("u_ModelMatrix", modelMatrix);
		// Render
		quadBatch->getArrayBuffer()->render();
	}

	void Renderer::endScene(float exposure, float gamma, const glm::mat3& kernel) {
		// Get window size
		auto windowSize = m_Window->getSize();
		// Copy antialiasing framebuffer to postprocessing framebuffer and bind context framebuffer
		m_GraphicsApi->copyFramebufferColorData(m_AntialiasingFramebuffer, m_PostprocessingFramebuffer, windowSize);
		m_GraphicsApi->unbindFramebuffer();
		// Render current texture to the postprocessing pipeline
		m_PostprocessingShader->activate();
		m_PostprocessingShader->setUniform("u_Result0", 0);
		m_PostprocessingShader->setUniform("u_Offset", m_KernelOffset);
		m_PostprocessingShader->setUniform("u_Gamma", gamma);
		m_PostprocessingShader->setUniform("u_Kernel", kernel);
		m_PostprocessingShader->setUniform("u_Exposure", exposure);
		m_PostprocessingFramebuffer->bindColorAttachment();
		m_PostprocessingBuffer->render();
	}

	void Renderer::setWindowSize(const glm::ivec2& windowSize) {
		// Update FBO sizes
		m_AntialiasingFramebuffer->setSize(windowSize);
		m_PostprocessingFramebuffer->setSize(windowSize);
	}

}