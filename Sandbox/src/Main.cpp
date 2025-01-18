#include <NeoVoxel.h>

const char* VS = R"(
	#version 330 core

	layout (location = 0) in vec2 i_Position;
	layout (location = 1) in vec2 i_TexCoord;

	out vec2 p_TexCoord;

	void main() {
		gl_Position = vec4(i_Position, 0.0, 1.0);
		p_TexCoord = i_TexCoord;
	}
)";

const char* FS = R"(
	#version 330 core

	in vec2 p_TexCoord;

	out vec4 o_FragColor;

	void main() {
		o_FragColor = vec4(p_TexCoord.x, p_TexCoord.y, 0.0, 1.0);
	}
)";

class SandboxLayer : public NeoVoxel::EventListenerLayer {

public:
	SandboxLayer() : EventListenerLayer("SandboxLayer") {}

	virtual void onCreate() override {

		NeoVoxel::ArrayBufferSpec arrayBufferSpec{
			{ NeoVoxel::ArrayBufferElement::PACKED_POSITION_UV_2D },
			NeoVoxel::ArrayBufferDrawType::STATIC
		};
		m_ArrayBuffer = NeoVoxel::Application::get().getGraphicsApi().createArrayBuffer(arrayBufferSpec);
		NeoVoxel::ShaderSpec shaderSpec{ VS, std::nullopt, FS };
		m_Shader = NeoVoxel::Application::get().getGraphicsApi().createShader(shaderSpec);

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