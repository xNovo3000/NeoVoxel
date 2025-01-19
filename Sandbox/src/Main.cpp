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

		std::vector<std::pair<glm::vec2, glm::vec2>> positionsAndUvs = {
			{ { -0.5F, -0.5F }, { 0.0F, 0.0F } },
			{ { -0.5F,  0.5F }, { 0.0F, 1.0F } },
			{ {  0.5F, -0.5F }, { 1.0F, 0.0F } },
			{ {  0.5F,  0.5F }, { 1.0F, 1.0F } }
		};
		std::vector<uint32_t> indices = { 0, 2, 1, 2, 3, 1 };

		m_ArrayBuffer->setVertexBufferData(0, positionsAndUvs);
		m_ArrayBuffer->setElementBufferData(indices);

	}

	virtual void onRender() override {
		EventListenerLayer::onRender();

		m_Shader->activate();
		m_ArrayBuffer->render();

	}

private:
	NeoVoxel::ArrayBufferRef m_ArrayBuffer;
	NeoVoxel::ShaderRef m_Shader;

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