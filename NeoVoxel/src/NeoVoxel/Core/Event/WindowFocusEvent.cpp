#include <PCH.h>
#include <NeoVoxel/Core/Event/WindowFocusEvent.h>

namespace NeoVoxel {

	WindowFocusEvent::WindowFocusEvent(bool focused) :
		Event(EventType::WINDOW_FOCUS), m_Focused(focused)
	{}

}