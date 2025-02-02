#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Core/Asset.h>
#include <NeoVoxel/Core/Input.h>
#include <NeoVoxel/Core/Time.h>
#include <NeoVoxel/Core/Window.h>
#include <NeoVoxel/Graphics/GraphicsApi.h>
#include <NeoVoxel/Graphics/Renderer.h>
#include <NeoVoxel/Layer.h>

#include <vector>

namespace NeoVoxel {

	class Application {

	private:
		static Application* INSTANCE;

	public:
		static Application& get() noexcept { return *INSTANCE; }

	public:
		Application();
		Application(const char* name);
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		Application(Application&&) noexcept = delete;
		Application& operator=(Application&&) noexcept = delete;

		void run();
		void stop();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);

		AssetLoader& getAssetLoader() noexcept { return m_AssetLoader; }
		Window& getWindow() noexcept { return *m_Window; }
		Input& getInput() noexcept { return *m_Input; }
		GraphicsApi& getGraphicsApi() noexcept { return *m_GraphicsApi; }
		Renderer& getRenderer() noexcept { return m_Renderer; }

		const char* getName() const noexcept { return m_Name; }

	private:
		bool m_IsRunning;
		const char* m_Name;

		Timepoint m_LastTime;

		std::vector<LayerPtr> m_LayerStack;
		std::vector<Layer*> m_LayersToCreate;
		std::vector<Layer*> m_LayersToDestroy;

		AssetLoader m_AssetLoader;
		WindowRef m_Window;
		InputRef m_Input;
		GraphicsApiRef m_GraphicsApi;
		Renderer m_Renderer;

	};

#define NV_GET_ASSET_LOADER		::NeoVoxel::Application::get().getAssetLoader()
#define NV_GET_WINDOW			::NeoVoxel::Application::get().getWindow()
#define NV_GET_INPUT			::NeoVoxel::Application::get().getInput()
#define NV_GET_GRAPHICS_API		::NeoVoxel::Application::get().getGraphicsApi()
#define NV_GET_RENDERER			::NeoVoxel::Application::get().getRenderer()

}