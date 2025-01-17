#pragma once

#include <NeoVoxel/Layer.h>

#include <NeoVoxel/Core/Event/CursorPositionEvent.h>
#include <NeoVoxel/Core/Event/KeyEvent.h>
#include <NeoVoxel/Core/Event/MouseButtonEvent.h>
#include <NeoVoxel/Core/Event/WindowCloseEvent.h>
#include <NeoVoxel/Core/Event/WindowFocusEvent.h>
#include <NeoVoxel/Core/Event/WindowSizeEvent.h>

namespace NeoVoxel {

	class EventListenerLayer : public Layer {

	public:
		EventListenerLayer();
		EventListenerLayer(const char* name);
		virtual ~EventListenerLayer() = default;

		EventListenerLayer(const EventListenerLayer&) = delete;
		EventListenerLayer& operator=(const EventListenerLayer&) = delete;

		EventListenerLayer(EventListenerLayer&&) noexcept = default;
		EventListenerLayer& operator=(EventListenerLayer&&) noexcept = default;

		virtual void onUpdate(Timestep timestep, std::vector<EventPtr>& events) override;

	protected:
		virtual bool onCursorPositionEvent(Timestep timestep, CursorPositionEvent& event);
		virtual bool onKeyEvent(Timestep timestep, KeyEvent& event);
		virtual bool onMouseButtonEvent(Timestep timestep, MouseButtonEvent& event);
		virtual bool onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event);
		virtual bool onWindowFocusEvent(Timestep timestep, WindowFocusEvent& event);
		virtual bool onWindowSizeEvent(Timestep timestep, WindowSizeEvent& event);

	};

}