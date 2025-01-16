#include <PCH.h>
#include <NeoVoxel/Core/Event/MouseButtonEvent.h>

namespace NeoVoxel {

	MouseButtonEvent::MouseButtonEvent(int32_t mouseButton, MouseButtonAction action, int32_t modifierButtons) :
		Event(EventType::MOUSE_BUTTON), m_MouseButton(mouseButton), m_Action(action), m_ModifierButtons(modifierButtons)
	{}

}