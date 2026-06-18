#pragma once

#include <neovoxel/core.hpp>

namespace neovoxel {

    class graphics_api : public named_resource {

    public:
        explicit graphics_api();
        explicit graphics_api(const char *_name);
        virtual ~graphics_api() = default;

    };

}