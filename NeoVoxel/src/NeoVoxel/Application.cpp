#include <PCH.h>
#include <NeoVoxel/Application.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	/* Application - Singleton */

	Application* Application::INSTANCE = nullptr;

	/* Application */

	Application::Application() : Application("Application") {}

	Application::Application(const char* name) : m_IsRunning(true), m_Name(name), m_LastTime(0),
		m_LayerStack(), m_LayersToCreate(), m_LayersToDestroy(),
		m_Window(new Window()), m_Input(new Input())
	{ INSTANCE = this; }

	Application::~Application() {}

	void Application::run() {
		NV_INFO("Starting {}", m_Name);
		while (m_IsRunning) {
			// Calculate timestep
			auto currentTime = m_Input->getCurrentTime();
			auto timestep = currentTime - m_LastTime;
			m_LastTime = currentTime;
			NV_TRACE("{}: current timestep {} ns", m_Name, currentTime.getNanoseconds());
			// Read window/input events
			auto events = m_Window->pollEvents();
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
			// Swap window buffers
			m_Window->swapBuffers();
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