#include <pch.hpp>
#include <neovoxel/graphics.hpp>

namespace neovoxel {

    graphics_api::graphics_api() : graphics_api("graphics_api") {}
    graphics_api::graphics_api(const char *_name) : named_resource(_name) {}

}