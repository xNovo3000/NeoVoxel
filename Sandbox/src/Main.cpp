#include <NeoVoxel.h>

class SandboxLayer : public NeoVoxel::EventListenerLayer {

public:
	SandboxLayer() : EventListenerLayer("SandboxLayer") {}

	virtual void onCreate() override {
		EventListenerLayer::onCreate();

		auto& graphicsApi = NeoVoxel::Application::get().getGraphicsApi();
		auto& assetLoader = NeoVoxel::Application::get().getAssetLoader();

		NeoVoxel::ArrayBufferSpec arrayBufferSpec{
			{ NeoVoxel::ArrayBufferElement::PACKED_POSITION_UV_2D },
			NeoVoxel::ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = graphicsApi.createArrayBuffer(arrayBufferSpec);

		auto maybeVertexShader = assetLoader.loadStringFile("asset/shader/2d.vert");
		auto maybeFragmentShader = assetLoader.loadStringFile("asset/shader/2d.frag");

		NeoVoxel::ShaderSpec shaderSpec{ *maybeVertexShader, std::nullopt, *maybeFragmentShader };
		m_Shader = graphicsApi.createShader(shaderSpec);

		auto maybeStoneImage = assetLoader.loadImage("asset/texture/stone.png");
		NeoVoxel::Texture2DSpec texture2DSpec{
			NeoVoxel::TextureColorFormat::RGB,
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

	virtual void onRender() override {
		EventListenerLayer::onRender();

		m_Shader->activate();
		m_Shader->setUniform(0, 0);
		m_Texture2D->bind();
		m_ArrayBuffer->render();

	}

private:
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