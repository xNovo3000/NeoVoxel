#include <PCH.h>
#include <NeoVoxel/Core/Event/CursorPositionEvent.h>

namespace NeoVoxel {

	CursorPositionEvent::CursorPositionEvent(const glm::vec2& position) :
		Event(EventType::CURSOR_POSITION), m_Position(position)
	{}

}