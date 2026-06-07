#include <pch.hpp>
#include <neovoxel/event.hpp>

namespace neovoxel {

    event::event(uint64_t _type) : _type(_type) {}

    void event::stop_propagation() { _propagate = 0; }

}