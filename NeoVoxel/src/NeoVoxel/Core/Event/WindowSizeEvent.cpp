#include <PCH.h>
#include <NeoVoxel/Core/Event/WindowSizeEvent.h>

namespace NeoVoxel {

	WindowSizeEvent::WindowSizeEvent(const glm::ivec2& size) :
		Event(EventType::WINDOW_SIZE), m_Size(size)
	{}

}