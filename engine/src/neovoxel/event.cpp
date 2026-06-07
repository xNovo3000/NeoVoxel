#include <pch.hpp>
#include <neovoxel/event.hpp>

namespace neovoxel {

    event::event(uint64_t _type) : _type(_type) {}

    void event::stop_propagation() { _propagate = 0; }

    window_close_event::window_close_event() : event(window_close_event::_type) {}
    window_focus_event::window_focus_event(bool _focused) : event(window_focus_event::_type), _focused(_focused) {}

}