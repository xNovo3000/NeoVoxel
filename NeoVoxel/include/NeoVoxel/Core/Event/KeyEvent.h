#pragma once

#include <NeoVoxel/Core/Event.h>

namespace NeoVoxel {

	enum class KeyAction : uint8_t { PRESS, RELEASE, REPEAT };

	class KeyEvent : public Event {

	public:
		KeyEvent(int32_t key, KeyAction action, int32_t modifierKeys);
		virtual ~KeyEvent() = default;

		KeyEvent(const KeyEvent&) = delete;
		KeyEvent& operator=(const KeyEvent&) = delete;

		KeyEvent(KeyEvent&&) noexcept = default;
		KeyEvent& operator=(KeyEvent&&) noexcept = default;

		int32_t getKey() const noexcept { return m_Key; }
		KeyAction getAction() const noexcept { return m_Action; }
		int32_t getModifierKeys() const noexcept { return m_ModifierKeys; }

		static EventType getStaticType() { return EventType::KEY; }

	private:
		int32_t m_Key;
		KeyAction m_Action;
		int32_t m_ModifierKeys;

	};

}