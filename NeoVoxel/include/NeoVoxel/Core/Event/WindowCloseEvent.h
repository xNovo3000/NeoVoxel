#pragma once

#include <NeoVoxel/Core/Event.h>

namespace NeoVoxel {

	class WindowCloseEvent : public Event {

	public:
		WindowCloseEvent();
		virtual ~WindowCloseEvent() = default;

		WindowCloseEvent(const WindowCloseEvent&) = delete;
		WindowCloseEvent& operator=(const WindowCloseEvent&) = delete;

		WindowCloseEvent(WindowCloseEvent&&) noexcept = default;
		WindowCloseEvent& operator=(WindowCloseEvent&&) noexcept = default;

		static inline EventType getStaticType() { return EventType::WINDOW_CLOSE; }

	};

}
