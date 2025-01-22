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
		auto maybeStoneImage = assetLoader.loadImage("asset/texture/stone.png");

		/* Create objects */

		NeoVoxel::ArrayBufferSpec arrayBufferSpec{
			{ NeoVoxel::ArrayBufferElement::PACKED_POSITION_UV_2D },
			NeoVoxel::ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = graphicsApi.createArrayBuffer(arrayBufferSpec);

		NeoVoxel::ShaderSpec shaderSpec{ *maybeVertexShader, std::nullopt, *maybeFragmentShader };
		m_Shader = graphicsApi.createShader(shaderSpec);

		NeoVoxel::Texture2DSpec texture2DSpec{
			NeoVoxel::TextureColorSpace::SDR,
			NeoVoxel::TextureMipmapGeneration::ENABLED,
			{
				{ NeoVoxel::TextureParamsName::FILTER_MAG, NeoVoxel::TextureParamsValue::FILTER_NEAREST },
				{ NeoVoxel::TextureParamsName::FILTER_MIN, NeoVoxel::TextureParamsValue::FILTER_LINEAR },
				{ NeoVoxel::TextureParamsName::WRAP_S, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE },
				{ NeoVoxel::TextureParamsName::WRAP_T, NeoVoxel::TextureParamsValue::WRAP_CLAMP_TO_EDGE }
			}
		};
		m_Texture2D = graphicsApi.createTexture2D(texture2DSpec);

		/* Update data */

		std::vector<std::pair<glm::vec2, glm::vec2>> positionsAndUvs = {
			{ { -0.5F, -0.5F }, { 0.0F, 0.0F } },
			{ { -0.5F,  0.5F }, { 0.0F, 1.0F } },
			{ {  0.5F, -0.5F }, { 1.0F, 0.0F } },
			{ {  0.5F,  0.5F }, { 1.0F, 1.0F } }
		};
		std::vector<uint32_t> indices = { 0, 2, 1, 2, 3, 1 };

		m_ArrayBuffer->setVertexBufferData(0, positionsAndUvs);
		m_ArrayBuffer->setElementBufferData(indices);

		m_Texture2D->update((*maybeStoneImage).m_Size, (*maybeStoneImage).m_Data);

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

		auto& window = NV_GET_WINDOW;
		auto aspectRatio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);

		m_Shader->activate();
		m_Shader->setUniform(0, m_Projection.getProjectionMatrix(aspectRatio));
		m_Shader->setUniform(1, m_Camera2D.getViewMatrix());
		m_Shader->setUniform(2, m_Transform.getModelMatrix());
		m_Shader->setUniform(3, 0);
		m_Texture2D->bind();
		m_ArrayBuffer->render();

	}

private:
	NeoVoxel::Camera2D m_Camera2D;
	NeoVoxel::OrthographicProjection m_Projection;
	NeoVoxel::Transform2D m_Transform;
	NeoVoxel::ArrayBufferRef m_ArrayBuffer;
	NeoVoxel::ShaderRef m_Shader;
	NeoVoxel::Texture2DRef m_Texture2D;

};

class SandboxApplication : public NeoVoxel::Application {

public:
	SandboxApplication() : Application("SandboxApplication") {
		pushLayer(new SandboxLayer());
	}

};

#include <NeoVoxel/Entrypoint.h>

NeoVoxel::Application* createApplication() {
	return new SandboxApplication();
}