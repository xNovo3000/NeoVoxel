#include <PCH.h>
#include <NeoVoxel/Layer/EventListenerLayer.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	EventListenerLayer::EventListenerLayer() : EventListenerLayer("EventListenerLayer") {}
	EventListenerLayer::EventListenerLayer(const char* name) : Layer(name) {}

	void EventListenerLayer::onUpdate(Timestep timestep, std::vector<EventPtr>& events) {
		Layer::onUpdate(timestep, events);
		for (auto& event : events) {
			dispatchEvent<CursorPositionEvent>(*event, std::bind(&EventListenerLayer::onCursorPositionEvent, this, timestep, std::placeholders::_1));
			dispatchEvent<KeyEvent>(*event, std::bind(&EventListenerLayer::onKeyEvent, this, timestep, std::placeholders::_1));
			dispatchEvent<MouseButtonEvent>(*event, std::bind(&EventListenerLayer::onMouseButtonEvent, this, timestep, std::placeholders::_1));
			dispatchEvent<WindowCloseEvent>(*event, std::bind(&EventListenerLayer::onWindowCloseEvent, this, timestep, std::placeholders::_1));
			dispatchEvent<WindowFocusEvent>(*event, std::bind(&EventListenerLayer::onWindowFocusEvent, this, timestep, std::placeholders::_1));
			dispatchEvent<WindowSizeEvent>(*event, std::bind(&EventListenerLayer::onWindowSizeEvent, this, timestep, std::placeholders::_1));
		}
	}

	bool EventListenerLayer::onCursorPositionEvent(Timestep timestep, CursorPositionEvent& event) {
		NV_TRACE(
			"{}: received CursorPositionEvent. X: {} - Y: {}",
			getName(), event.getPosition().x, event.getPosition().y
		);
		return false;
	}

	bool EventListenerLayer::onKeyEvent(Timestep timestep, KeyEvent& event) {
		NV_TRACE(
			"{}: received KeyEvent. KeyID: {}, Action: {}, ModifierKeysID: {}",
			getName(), event.getKey(), fmt::underlying(event.getAction()), event.getModifierKeys()
		);
		return false;
	}

	bool EventListenerLayer::onMouseButtonEvent(Timestep timestep, MouseButtonEvent& event) {
		NV_TRACE(
			"{}: received MouseButtonEvent. MouseButtonID: {}, Action: {}, ModifierButtonsID: {}",
			getName(), event.getMouseButton(), fmt::underlying(event.getAction()), event.getModifierButtons()
		);
		return false;
	}

	bool EventListenerLayer::onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event) {
		NV_TRACE("{}: received WindowCloseEvent", getName());
		return false;
	}

	bool EventListenerLayer::onWindowFocusEvent(Timestep timestep, WindowFocusEvent& event) {
		NV_TRACE("{}: received WindowFocusEvent. Focused: {}", getName(), event.isFocused());
		return false;
	}

	bool EventListenerLayer::onWindowSizeEvent(Timestep timestep, WindowSizeEvent& event) {
		NV_TRACE(
			"{}: received WindowSizeEvent. X: {} - Y: {}",
			getName(), event.getSize().x, event.getSize().y
		);
		return false;
	}

}