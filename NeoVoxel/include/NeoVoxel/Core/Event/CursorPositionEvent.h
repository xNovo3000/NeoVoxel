#pragma once

#include <NeoVoxel/Core/Event.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class CursorPositionEvent : public Event {

	public:
		CursorPositionEvent(const glm::vec2& position);
		virtual ~CursorPositionEvent() = default;

		CursorPositionEvent(const CursorPositionEvent&) = delete;
		CursorPositionEvent& operator=(const CursorPositionEvent&) = delete;

		CursorPositionEvent(CursorPositionEvent&&) noexcept = default;
		CursorPositionEvent& operator=(CursorPositionEvent&&) noexcept = default;

		const glm::vec2& getPosition() const noexcept { return m_Position; }

		static EventType getStaticType() { return EventType::CURSOR_POSITION; }

	private:
		glm::vec2 m_Position;

	};

}