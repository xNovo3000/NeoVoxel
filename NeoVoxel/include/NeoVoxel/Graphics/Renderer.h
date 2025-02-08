#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Core/Asset.h>
#include <NeoVoxel/Core/Window.h>
#include <NeoVoxel/Graphics/ArrayBuffer.h>
#include <NeoVoxel/Graphics/Camera2D.h>
#include <NeoVoxel/Graphics/Framebuffer.h>
#include <NeoVoxel/Graphics/GraphicsApi.h>
#include <NeoVoxel/Graphics/Material.h>
#include <NeoVoxel/Graphics/Projection.h>
#include <NeoVoxel/Graphics/QuadBatch.h>
#include <NeoVoxel/Graphics/Shader.h>
#include <NeoVoxel/Graphics/Transform2D.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	constexpr float DEFAULT_EXPOSURE = 1.0F;
	constexpr float DEFAULT_GAMMA = 2.2F;
	constexpr glm::mat3 DEFAULT_KERNEL = glm::mat3(0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F);

	class Renderer {

	public:
		Renderer(const WindowRef& window, const GraphicsApiRef& graphicsApi, const AssetLoader& assetLoader);

		void beginScene(const Camera2D& camera, const Projection& projection);
		// void begin(const Camera3D& camera, const Projection& projection);

		void submit(const QuadBatchRef& quadBatch, const MaterialRef& material, const Transform2D& transform);
		// void submit(const Mesh& mesh, const MaterialRef& material, const Transform3D& transform);

		void endScene(float exposure = DEFAULT_EXPOSURE, float gamma = DEFAULT_GAMMA, const glm::mat3& kernel = DEFAULT_KERNEL);

		void setWindowSize(const glm::ivec2& windowSize);

	private:
		WindowRef m_Window;
		GraphicsApiRef m_GraphicsApi;
		FramebufferRef m_AntialiasingFramebuffer;
		FramebufferRef m_PostprocessingFramebuffer;
		ArrayBufferRef m_PostprocessingBuffer;
		ShaderRef m_PostprocessingShader;
		glm::mat4 m_ProjectionViewMatrix = glm::mat4(1.0F);
		glm::vec2 m_KernelOffset = glm::vec2(0.0F);

	};

}