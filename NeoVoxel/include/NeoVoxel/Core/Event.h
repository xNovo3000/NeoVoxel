#pragma once

#include <NeoVoxel/Core/Base.h>

namespace NeoVoxel {

	enum class EventType : uint8_t { CURSOR_POSITION, KEY, MOUSE_BUTTON, WINDOW_CLOSE, WINDOW_FOCUS, WINDOW_SIZE };

	class Event {

	public:
		Event(EventType type);
		virtual ~Event() = default;

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		Event(Event&&) noexcept = default;
		Event& operator=(Event&&) noexcept = default;

		void stopPropagation();

		bool shouldPropagate() const noexcept { return m_ShouldPropagate; }
		EventType getType() const noexcept { return m_Type; }

	private:
		bool m_ShouldPropagate;
		EventType m_Type;

	};

	using EventPtr = Ptr<Event>;

}