#include <PCH.h>
#include <NeoVoxel/Core/Event/KeyEvent.h>

namespace NeoVoxel {

	KeyEvent::KeyEvent(int32_t key, KeyAction action, int32_t modifierKeys) : Event(EventType::KEY),
		m_Key(key), m_Action(action), m_ModifierKeys(modifierKeys)
	{}

}