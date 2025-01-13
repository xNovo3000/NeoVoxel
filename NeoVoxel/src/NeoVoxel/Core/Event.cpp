#include <PCH.h>
#include <NeoVoxel/Core/Event.h>

namespace NeoVoxel {

	Event::Event(EventType type) : m_ShouldPropagate(true), m_Type(type) {}

	void Event::stopPropagation() { m_ShouldPropagate = false; }

}