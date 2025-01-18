#include <PCH.h>
#include "BaseLayer.h"
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	BaseLayer::BaseLayer() : EventListenerLayer("BaseLayer") {}

	bool BaseLayer::onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event) {
		EventListenerLayer::onWindowCloseEvent(timestep, event);
		Application::get().stop();
		return true;
	}

}