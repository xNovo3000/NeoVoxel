#include <PCH.h>
#include <NeoVoxel/Application.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	/* Application - Singleton */

	Application* Application::INSTANCE = nullptr;

	/* Application */

	Application::Application() : Application("Application") {}

	Application::Application(const char* name) : m_IsRunning(true), m_Name(name),
		m_LayerStack(), m_LayersToCreate(), m_LayersToDestroy()
	{ INSTANCE = this; }

	Application::~Application() {}

	void Application::run() {
		NV_INFO("Starting {}", m_Name);
		while (m_IsRunning) {
			// TODO: Calculate timestep
			auto timestep = Timestep(0.0);
			// TODO: Read window/input events
			auto events = std::vector<EventPtr>();
			// LayerStack update (reverse order)
			for (auto& layer : m_LayerStack | std::views::reverse) {
				layer->onUpdate(timestep, events);
				// Remove events that must not be propagated to the next layer
				std::erase_if(events, [](EventPtr& event) { return !event->shouldPropagate(); });
			}
			// LayerStack render
			for (auto& layer : m_LayerStack) {
				layer->onRender();
			}
			// TODO: Add new layers to the stack
			// TODO: Remove layers from the stack
			// TODO: Swap window buffers
		}
	}

	void Application::stop() {
		m_IsRunning = false;
		for (auto& layer : m_LayerStack) {
			m_LayersToDestroy.push_back(layer.get());
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_LayersToCreate.push_back(layer);
	}

	void Application::popLayer(Layer* layer) {
		m_LayersToDestroy.push_back(layer);
	}

}