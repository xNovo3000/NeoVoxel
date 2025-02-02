#include <PCH.h>
#include "BaseLayer.h"
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	BaseLayer::BaseLayer() : EventListenerLayer("BaseLayer") {}

	void BaseLayer::onRender() {
		EventListenerLayer::onRender();
		auto& graphicsApi = NV_GET_GRAPHICS_API;
		graphicsApi.clearColor();
		graphicsApi.clearDepth();
	}

	bool BaseLayer::onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event) {
		EventListenerLayer::onWindowCloseEvent(timestep, event);
		Application::get().stop();
		return true;
	}

	bool BaseLayer::onWindowSizeEvent(Timestep timestep, WindowSizeEvent& event) {
		EventListenerLayer::onWindowSizeEvent(timestep, event);
		NV_GET_GRAPHICS_API.setViewport(event.getSize());
		NV_GET_RENDERER.setWindowSize(event.getSize());
		return false;
	}

}