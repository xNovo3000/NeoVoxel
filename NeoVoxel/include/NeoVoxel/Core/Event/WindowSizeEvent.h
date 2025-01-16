#pragma once

#include <NeoVoxel/Core/Event.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class WindowSizeEvent : public Event {

	public:
		WindowSizeEvent(const glm::ivec2& size);
		virtual ~WindowSizeEvent() = default;

		WindowSizeEvent(const WindowSizeEvent&) = delete;
		WindowSizeEvent& operator=(const WindowSizeEvent&) = delete;

		WindowSizeEvent(WindowSizeEvent&&) noexcept = default;
		WindowSizeEvent& operator=(WindowSizeEvent&&) noexcept = default;

		const glm::ivec2& getSize() const noexcept { return m_Size; }

		static inline EventType getStaticType() { return EventType::WINDOW_SIZE; }

	private:
		glm::ivec2 m_Size;

	};

}