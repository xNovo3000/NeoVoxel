#pragma once

#include <NeoVoxel/Core/Event.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class WindowFocusEvent : public Event {

	public:
		WindowFocusEvent(bool focused);
		virtual ~WindowFocusEvent() = default;

		WindowFocusEvent(const WindowFocusEvent&) = delete;
		WindowFocusEvent& operator=(const WindowFocusEvent&) = delete;

		WindowFocusEvent(WindowFocusEvent&&) noexcept = default;
		WindowFocusEvent& operator=(WindowFocusEvent&&) noexcept = default;

		bool isFocused() const noexcept { return m_Focused; }

		static EventType getStaticType() { return EventType::WINDOW_FOCUS; }

	private:
		bool m_Focused;

	};

}