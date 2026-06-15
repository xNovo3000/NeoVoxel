#include <pch.hpp>
#include <neovoxel/event.hpp>

#include <neovoxel/debug.hpp>

namespace neovoxel {

    event::event(uint64_t _type) : _propagate(1), _type(_type) {}

    void event::stop_propagation() {
        NV_LOG_DEBUG("Event type '{}': stopped propagation", static_cast<uint64_t>(_type));
        _propagate = 0;
    }

    window_close_event::window_close_event() : event(window_close_event::_type) {}
    window_focus_event::window_focus_event(bool _focused) : event(window_focus_event::_type), _focused(_focused) {}
    window_size_event::window_size_event(glm::ivec2 _size) : event(window_size_event::_type), _size(_size) {}
    cursor_position_event::cursor_position_event(glm::vec2 _position) : event(cursor_position_event::_type), _position(_position) {}
    key_event::key_event(int32_t _key, int32_t _modifiers, key_action _action) : event(key_event::_type), _key(_key), _modifiers(_modifiers), _action(_action) {}
    mouse_button_event::mouse_button_event(int32_t _button, int32_t _modifiers, mouse_button_action _action) : event(mouse_button_event::_type), _button(_button), _modifiers(_modifiers), _action(_action) {}

}