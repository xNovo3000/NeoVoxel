#pragma once

#include <NeoVoxel/Core/Event.h>

namespace NeoVoxel {

	enum class MouseButtonAction : uint8_t { PRESS, RELEASE, REPEAT };

	class MouseButtonEvent : public Event {

	public:
		MouseButtonEvent(int32_t MouseButton, MouseButtonAction action, int32_t modifierButtons);
		virtual ~MouseButtonEvent() = default;

		MouseButtonEvent(const MouseButtonEvent&) = delete;
		MouseButtonEvent& operator=(const MouseButtonEvent&) = delete;

		MouseButtonEvent(MouseButtonEvent&&) noexcept = default;
		MouseButtonEvent& operator=(MouseButtonEvent&&) noexcept = default;

		int32_t getMouseButton() const noexcept { return m_MouseButton; }
		MouseButtonAction getAction() const noexcept { return m_Action; }
		int32_t getModifierButtons() const noexcept { return m_ModifierButtons; }

		static EventType getStaticType() { return EventType::MOUSE_BUTTON; }

	private:
		int32_t m_MouseButton;
		MouseButtonAction m_Action;
		int32_t m_ModifierButtons;

	};

}