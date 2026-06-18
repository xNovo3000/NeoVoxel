#pragma once

#include <neovoxel/core.hpp>

#include <glm/glm.hpp>

#include <memory>

namespace neovoxel {

#if NV_BUILD_TYPE == NV_BUILD_TYPE_DEBUG
    constexpr glm::vec4 _graphics_default_clear_color { 0.75F, 0.125F, 0.65F, 0.0F };
#else
    constexpr glm::vec4 _graphics_default_clear_color { 0.0F, 0.0F, 0.0F, 0.0F };
#endif
    constexpr float _graphics_default_clear_depth = 1.0F;
    constexpr int32_t _graphics_default_clear_stencil = 0;

    enum class graphics_capability {};

    class graphics_api : public named_resource {

    public:
        explicit graphics_api();
        explicit graphics_api(const char *_name);
        virtual ~graphics_api() = default;

        // Non-copyable but movable, required for smart pointers
        graphics_api(const graphics_api&) = delete;
        graphics_api(graphics_api&&) noexcept = default;
        graphics_api &operator=(graphics_api&&) noexcept = default;

        virtual void clear_color(const glm::vec4 &_color = _graphics_default_clear_color);
        virtual void clear_depth(double _depth = _graphics_default_clear_depth);
        virtual void clear_stencil(int32_t _stencil = _graphics_default_clear_stencil);

        virtual void disable(graphics_capability _capability);
        virtual void enable(graphics_capability _capability);

        virtual void set_viewport(const glm::ivec2 &_size);

    };

    using graphics_api_ptr = std::unique_ptr<graphics_api>;

}