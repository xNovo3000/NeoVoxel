#include <PCH.h>
#include "BaseLayer.h"
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	BaseLayer::BaseLayer() : EventListenerLayer("BaseLayer") {}

	void BaseLayer::onRender() {
		EventListenerLayer::onRender();
		auto& graphicsApi = Application::get().getGraphicsApi();
		graphicsApi.clearColor();
		graphicsApi.clearDepth();
	}

	bool BaseLayer::onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event) {
		EventListenerLayer::onWindowCloseEvent(timestep, event);
		Application::get().stop();
		return true;
	}

}