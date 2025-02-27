#include <NeoVoxel.h>

class SandboxLayer : public NeoVoxel::EventListenerLayer {

public:
	SandboxLayer() : EventListenerLayer("SandboxLayer") {}

	virtual void onCreate() override {
		EventListenerLayer::onCreate();

		/* Get instances */

		auto& graphicsApi = NV_GET_GRAPHICS_API;
		auto& assetLoader = NV_GET_ASSET_LOADER;

		/* Load files */

		auto maybeVertexShader = assetLoader.loadStringFile("asset/shader/2d.vert");
		auto maybeFragmentShader = assetLoader.loadStringFile("asset/shader/2d.frag");
		auto maybePostprocessingVertexShader = assetLoader.loadStringFile("asset/shader/post.vert");
		auto maybePostprocessingFragmentShader = assetLoader.loadStringFile("asset/shader/post.frag");
		auto maybeStoneImage = assetLoader.loadImage("asset/texture/stone.png");

		/* Create objects */

		NeoVoxel::ArrayBufferSpec arrayBufferSpec{
			{ NeoVoxel::ArrayBufferElement::PACKED_POSITION_UV_2D },
			NeoVoxel::ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = graphicsApi.createArrayBuffer(arrayBufferSpec);
		m_PostprocessingBuffer = graphicsApi.createArrayBuffer(arrayBufferSpec);

		NeoVoxel::ShaderSpec shaderSpec{ *maybeVertexShader, std::nullopt, *maybeFragmentShader };
		m_Shader = graphicsApi.createShader(shaderSpec);
		NeoVoxel::ShaderSpec postprocessingShaderSpec{ *maybePostprocessingVertexShader, std::nullopt, *maybePostprocessingFragmentShader };
		m_PostprocessingShader = graphicsApi.createShader(postprocessingShaderSpec);

		NeoVoxel::Texture2DSpec texture2DSpec{
			NeoVoxel::TextureChannels::RGBA_8,
			NeoVoxel::TextureMipmapGeneration::ENABLED,
			{
				{ NeoVoxel::TextureParamsName::FILTER_MAG, NeoVoxel::TextureParamsValue::FILTER_NEAREST },
				{ NeoVoxel::TextureParamsName::FILTER_MIN, NeoVoxel::TextureParamsValue::FILTER_LINEAR },
				{ NeoVoxel::TextureParamsName::WRAP_S, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE },
				{ NeoVoxel::TextureParamsName::WRAP_T, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE }
			}
		};
		m_Texture2D = graphicsApi.createTexture2D(texture2DSpec);

		NeoVoxel::FramebufferSpec antialiasingFramebufferSpec{
			NeoVoxel::FramebufferColorChannels::RGBA_16,
			NeoVoxel::FramebufferColorType::BUFFER_MSAA,
			NeoVoxel::FramebufferDepthType::BUFFER_MSAA,
			{ 960, 540 }
		};
		m_AntialiasingFramebuffer = graphicsApi.createFramebuffer(antialiasingFramebufferSpec);

		NeoVoxel::FramebufferSpec postprocessingFramebufferSpec{
			NeoVoxel::FramebufferColorChannels::RGBA_16,
			NeoVoxel::FramebufferColorType::TEXTURE,
			NeoVoxel::FramebufferDepthType::NONE,
			{ 960, 540 }
		};
		m_PostprocessingFramebuffer = graphicsApi.createFramebuffer(postprocessingFramebufferSpec);

		/* Update data */

		std::vector<std::pair<glm::vec2, glm::vec2>> positionsAndUvs = {
			{ { -0.5F, -0.5F }, { 0.0F, 0.0F } },
			{ { -0.5F,  0.5F }, { 0.0F, 1.0F } },
			{ {  0.5F, -0.5F }, { 1.0F, 0.0F } },
			{ {  0.5F,  0.5F }, { 1.0F, 1.0F } }
		};
		std::vector<std::pair<glm::vec2, glm::vec2>> postprocessingPositionsAndUvs = {
			{ { -1.0F, -1.0F }, { 0.0F, 0.0F } },
			{ { -1.0F,  1.0F }, { 0.0F, 1.0F } },
			{ {  1.0F, -1.0F }, { 1.0F, 0.0F } },
			{ {  1.0F,  1.0F }, { 1.0F, 1.0F } }
		};
		std::vector<uint32_t> indices = { 0, 2, 1, 2, 3, 1 };

		m_ArrayBuffer->setVertexBufferData(0, positionsAndUvs);
		m_ArrayBuffer->setElementBufferData(indices);
		m_PostprocessingBuffer->setVertexBufferData(0, postprocessingPositionsAndUvs);
		m_PostprocessingBuffer->setElementBufferData(indices);

		m_Texture2D->update((*maybeStoneImage).m_Size, (*maybeStoneImage).m_Data, NeoVoxel::TextureDataChannels::RGBA);

	}

	virtual void onUpdate(NeoVoxel::Timestep timestep, std::vector<NeoVoxel::EventPtr>& events) override {
		EventListenerLayer::onUpdate(timestep, events);

		auto& input = NV_GET_INPUT;

		auto cameraMovement = glm::vec2(0.0F);
		if (input.isKeyPressed(NV_KEY_W)) {
			// Up
			cameraMovement.y += (float) timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_S)) {
			// Down
			cameraMovement.y -= (float) timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_D)) {
			// Left
			cameraMovement.x += (float) timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_A)) {
			// Right
			cameraMovement.x -= (float) timestep.deltaSeconds();
		}
		m_Camera2D.setPosition(m_Camera2D.getPosition() + cameraMovement);

	}

	virtual void onRender() override {
		EventListenerLayer::onRender();

		auto& graphicsApi = NV_GET_GRAPHICS_API;
		auto& window = NV_GET_WINDOW;

		auto windowSize = window.getSize();
		auto aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

		m_AntialiasingFramebuffer->bind();

		graphicsApi.clearColor();
		graphicsApi.clearDepth();

		m_Shader->activate();
		m_Shader->setUniform("u_ProjectionViewMatrix", m_Projection.getProjectionMatrix(aspectRatio) * m_Camera2D.getViewMatrix());
		m_Shader->setUniform("u_ModelMatrix", m_Transform.getModelMatrix());
		m_Shader->setUniform("u_Diffuse0", 0);
		m_Texture2D->bind();
		m_ArrayBuffer->render();

		graphicsApi.copyFramebufferColorData(m_AntialiasingFramebuffer, m_PostprocessingFramebuffer, windowSize);
		graphicsApi.unbindFramebuffer();

		m_PostprocessingShader->activate();
		m_PostprocessingShader->setUniform(0, 0);
		m_PostprocessingFramebuffer->bindColorAttachment();
		m_PostprocessingBuffer->render();

	}

	virtual bool onWindowSizeEvent(NeoVoxel::Timestep timestep, NeoVoxel::WindowSizeEvent& event) override {
		EventListenerLayer::onWindowSizeEvent(timestep, event);
		m_AntialiasingFramebuffer->setSize(event.getSize());
		m_PostprocessingFramebuffer->setSize(event.getSize());
		return false;
	}

private:
	NeoVoxel::Camera2D m_Camera2D;
	NeoVoxel::OrthographicProjection m_Projection;
	NeoVoxel::Transform2D m_Transform;
	NeoVoxel::ArrayBufferRef m_ArrayBuffer, m_PostprocessingBuffer;
	NeoVoxel::ShaderRef m_Shader, m_PostprocessingShader;
	NeoVoxel::Texture2DRef m_Texture2D;
	NeoVoxel::FramebufferRef m_PostprocessingFramebuffer, m_AntialiasingFramebuffer;

};

class RenderingLayer : public NeoVoxel::EventListenerLayer {

public:
	RenderingLayer() : EventListenerLayer("RenderingLayer") {}

	virtual void onCreate() override {
		EventListenerLayer::onCreate();

		auto& graphicsApi = NV_GET_GRAPHICS_API;
		auto& assetLoader = NV_GET_ASSET_LOADER;

		auto maybeVertexShader = assetLoader.loadStringFile("asset/shader/2d.vert");
		auto maybeFragmentShader = assetLoader.loadStringFile("asset/shader/2d.frag");
		auto maybeStoneImage = assetLoader.loadImage("asset/texture/stone.png");

		NeoVoxel::ShaderSpec quadShaderSpec{
			*maybeVertexShader, std::nullopt, *maybeFragmentShader
		};
		auto quadShader = graphicsApi.createShader(quadShaderSpec);

		NeoVoxel::Texture2DSpec quadTextureSpec{
			NeoVoxel::TextureChannels::SRGBA_8,
			NeoVoxel::TextureMipmapGeneration::ENABLED,
			{
				{ NeoVoxel::TextureParamsName::FILTER_MAG, NeoVoxel::TextureParamsValue::FILTER_NEAREST },
				{ NeoVoxel::TextureParamsName::FILTER_MIN, NeoVoxel::TextureParamsValue::FILTER_LINEAR },
				{ NeoVoxel::TextureParamsName::WRAP_S, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE },
				{ NeoVoxel::TextureParamsName::WRAP_T, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE }
			}
		};
		auto quadTexture = graphicsApi.createTexture2D(quadTextureSpec);
		quadTexture->update((*maybeStoneImage).m_Size, (*maybeStoneImage).m_Data, NeoVoxel::TextureDataChannels::RGBA);

		NeoVoxel::BasicTexturizedMaterialSpec materialSpec{
			quadShader, quadTexture
		};
		m_QuadMaterial = NeoVoxel::BasicTexturizedMaterial::create(materialSpec);

		std::vector<std::pair<glm::vec2, glm::vec2>> positionsAndUvs = {
			{ { -0.5F, -0.5F }, { 0.0F, 0.0F } },
			{ { -0.5F,  0.5F }, { 0.0F, 1.0F } },
			{ {  0.5F, -0.5F }, { 1.0F, 0.0F } },
			{ {  0.5F,  0.5F }, { 1.0F, 1.0F } }
		};
		std::vector<uint32_t> indices = { 0, 2, 1, 2, 3, 1 };
		m_QuadBuffer = NeoVoxel::DefaultQuadBatch::create();
		m_QuadBuffer->updateVertices(positionsAndUvs);
		m_QuadBuffer->updateIndices(indices);

	}

	virtual void onUpdate(NeoVoxel::Timestep timestep, std::vector<NeoVoxel::EventPtr>& events) override {
		EventListenerLayer::onUpdate(timestep, events);

		auto& input = NV_GET_INPUT;

		auto cameraMovement = glm::vec2(0.0F);
		float cameraRoll = 0.0F;
		if (input.isKeyPressed(NV_KEY_W)) {
			// Up
			cameraMovement.y += (float)timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_S)) {
			// Down
			cameraMovement.y -= (float)timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_D)) {
			// Left
			cameraMovement.x += (float)timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_A)) {
			// Right
			cameraMovement.x -= (float)timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_Z)) {
			// Right
			cameraRoll -= (float)timestep.deltaSeconds();
		}
		if (input.isKeyPressed(NV_KEY_X)) {
			// Right
			cameraRoll += (float)timestep.deltaSeconds();
		}
		m_Camera.setPosition(m_Camera.getPosition() + cameraMovement);
		m_Camera.setRoll(m_Camera.getRoll() + cameraRoll);

	}

	virtual void onRender() override {
		EventListenerLayer::onRender();

		auto& renderer = NV_GET_RENDERER;

		renderer.beginScene(m_Camera, m_Projection);
		renderer.submit(m_QuadBuffer, m_QuadMaterial, m_QuadTransform);
		renderer.endScene();

	}

private:
	NeoVoxel::QuadBatchRef m_QuadBuffer;
	NeoVoxel::MaterialRef m_QuadMaterial;
	NeoVoxel::Transform2D m_QuadTransform;
	NeoVoxel::Camera2D m_Camera;
	NeoVoxel::OrthographicProjection m_Projection;

};

class SandboxApplication : public NeoVoxel::Application {

public:
	SandboxApplication() : Application("SandboxApplication") {
		// pushLayer(new SandboxLayer());
		pushLayer(new RenderingLayer());
	}

};

#include <NeoVoxel/Entrypoint.h>

NeoVoxel::Application* createApplication() {
	return new SandboxApplication();
}