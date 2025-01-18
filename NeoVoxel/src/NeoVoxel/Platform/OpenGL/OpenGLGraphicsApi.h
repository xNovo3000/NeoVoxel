#pragma once

#include <NeoVoxel/Graphics/GraphicsApi.h>

namespace NeoVoxel {

	using OpenGLLoaderPtr = void (*(*)(const char*))(void);

	class OpenGLGraphicsApi : public GraphicsApi {

	public:
		OpenGLGraphicsApi(OpenGLLoaderPtr loaderFunction);
		virtual ~OpenGLGraphicsApi() = default;

		OpenGLGraphicsApi(const OpenGLGraphicsApi&) = delete;
		OpenGLGraphicsApi& operator=(const OpenGLGraphicsApi&) = delete;

		OpenGLGraphicsApi(OpenGLGraphicsApi&&) noexcept = default;
		OpenGLGraphicsApi& operator=(OpenGLGraphicsApi&&) noexcept = default;

		virtual void clearColor(const glm::vec4& color) override;
		virtual void clearDepth(float depth) override;

		virtual void setViewport(const glm::ivec2& viewport) override;

	};

}