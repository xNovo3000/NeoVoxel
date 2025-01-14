#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Layer.h>

#include <vector>

namespace NeoVoxel {

	class Application {

	private:
		static Application* INSTANCE;

	public:
		static inline Application& get() noexcept { return *INSTANCE; }

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

		const char* getName() const noexcept { return m_Name; }

	private:
		bool m_IsRunning;
		const char* m_Name;

		std::vector<LayerPtr> m_LayerStack;
		std::vector<Layer*> m_LayersToCreate;
		std::vector<Layer*> m_LayersToDestroy;

	};

}