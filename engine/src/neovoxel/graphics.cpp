#include <pch.hpp>
#include <neovoxel/graphics.hpp>

namespace neovoxel {

    graphics_api::graphics_api() : graphics_api("graphics_api") {}
    graphics_api::graphics_api(const char *_name) : named_resource(_name) {}

    void graphics_api::clear_color(const glm::vec4 &_color) {}
    void graphics_api::clear_depth(double _depth) {}
    void graphics_api::clear_stencil(int32_t _stencil) {}

    void graphics_api::disable(graphics_capability _capability) {}
    void graphics_api::enable(graphics_capability _capability) {}

    void graphics_api::set_viewport(const glm::ivec2 &_size) {}

}