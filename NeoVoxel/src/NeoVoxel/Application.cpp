#include <PCH.h>
#include <NeoVoxel/Application.h>
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#include "Platform/Common/BaseLayer.h"

#if NV_PLATFORM == NV_PLATFORM_DESKTOP
	#include "Platform/Desktop/GlfwWindow.h"
	#include "Platform/Desktop/GlfwInput.h"
	#include "Platform/OpenGL/OpenGLGraphicsApi.h"
#endif

namespace NeoVoxel {

	/* Application - Singleton */

	Application* Application::INSTANCE = nullptr;

	/* Application */

	Application::Application() : Application("Application") {}

	Application::Application(const char* name) : m_IsRunning(true), m_Name(name), m_LastTime(0),
		m_LayerStack(), m_LayersToCreate(), m_LayersToDestroy(),
		m_AssetLoader(),
#if NV_PLATFORM == NV_PLATFORM_DESKTOP
		m_Window(new GlfwWindow({ "NeoVoxel", { 960, 540 }, 60 })),
		m_Input(new GlfwInput(reinterpret_cast<GlfwWindow&>(*m_Window).getHandle())),
		m_GraphicsApi(new OpenGLGraphicsApi(reinterpret_cast<GlfwWindow&>(*m_Window).getContextLoaderPtr())),
#else
		m_Window(new Window()),
		m_Input(new Input()),
		m_GraphicsApi(new GraphicsApi()),
#endif
		m_Renderer(m_Window, m_GraphicsApi, m_AssetLoader)
	{
		INSTANCE = this;
		pushLayer(new BaseLayer());
	}

	Application::~Application() {}

	void Application::run() {
		NV_INFO("Starting {}", m_Name);
		// Assign above normal affinity in the scheduler
#ifdef _WIN32
		SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif
		// Loop
		while (m_IsRunning) {
			NV_PROFILE;
			// Calculate timestep
			auto currentTime = m_Input->getCurrentTime();
			auto timestep = currentTime - m_LastTime;
			m_LastTime = currentTime;
			// Read window/input events
			auto events = m_Window->pollEvents();
			// Trace current time, events and GPU memory usage
			NV_TRACE("{}: {} events in {} ms", m_Name, events.size(), timestep.deltaMilliseconds());
			NV_PROFILE_MEMORY_PRINT;
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
			// Get layers to create and destroy
			std::vector<Layer*> layersToCreate;
			std::vector<Layer*> layersToDestroy;
			std::swap(m_LayersToCreate, layersToCreate);
			std::swap(m_LayersToDestroy, layersToDestroy);
			// Add new layers to the stack. TODO: Optimize this routine
			for (auto layerToCreate : layersToCreate) {
				// Call onCover on the last layer
				if (!m_LayerStack.empty()) {
					m_LayerStack.back()->onCover();
				}
				// Add layer and call onCreate and onVisible
				layerToCreate->onCreate();
				layerToCreate->onVisible();
				m_LayerStack.emplace_back(layerToCreate);
			}
			// Remove layers from the stack
			for (auto layerToDestroy : layersToDestroy) {
				// Find layer in the stack
				auto maybeFound = std::find_if(
					m_LayerStack.begin(), m_LayerStack.end(),
					[layerToDestroy](LayerPtr& layer) { return layerToDestroy == layer.get(); }
				);
				// Case 1: layer does not exists. Trigger an error in the console
				// Case 2: layer is on top of the stack. Call onCover and onDestroy on that
				//         layer and call onVisible on the underlying layer if exists
				// Case 3: layer is in the middle of the stack. Call onDestroy
				if (maybeFound == m_LayerStack.end()) {
					// Case 1
					NV_ERROR(
						"{}: pointer of layer to destroy {} not found in the stack",
						m_Name, fmt::ptr(layerToDestroy)
					);
				}
				else if (maybeFound == m_LayerStack.end() - 1) {
					// Case 2
					(*maybeFound)->onCover();
					(*maybeFound)->onDestroy();
					m_LayerStack.erase(maybeFound);
					if (!m_LayerStack.empty()) {
						m_LayerStack.back()->onVisible();
					}
				}
				else {
					// Case 3
					(*maybeFound)->onDestroy();
					m_LayerStack.erase(maybeFound);
				}
			}
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