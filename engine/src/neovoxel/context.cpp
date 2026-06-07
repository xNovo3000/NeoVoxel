#include <pch.hpp>
#include <neovoxel/context.hpp>

namespace neovoxel {

    window::window() : window("window") {}
    window::window(const char *_name) : named_resource(_name) {}

    std::vector<event_ptr> window::pull_events() { return {}; }
    void window::swap_buffers() {}

    void window::title(const char *_title) {}

    void window::size(glm::ivec2 _size) {}
    glm::ivec2 window::size() const { return {}; }

    void window::refresh_rate(uint32_t _rate) {}
    uint32_t window::refresh_rate() const { return 0; }

}