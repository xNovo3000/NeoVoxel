#pragma once

#include <neovoxel/core.hpp>
#include <neovoxel/event.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace neovoxel {
    
    class window : public named_resource {

    public:
        window();
        window(const char *_name);
        virtual ~window() = default;

        // Non-copyable but movable, required for smart pointers
        window(const window&) = delete;
        window(window&&) noexcept = default;
        window &operator=(window&&) noexcept = default;

        virtual std::vector<event_ptr> poll_events();
        virtual void swap_buffers();

        virtual void title(const char *_title);

        virtual void size(glm::ivec2 _size);
        virtual glm::ivec2 size() const;

        virtual void refresh_rate(uint32_t _rate);
        virtual uint32_t refresh_rate() const;

    };

    using window_ptr = std::unique_ptr<window>;

    enum class input_cursor_mode { normal, disabled };

    class input : public named_resource {

    public:
        input();
        input(const char *_name);
        virtual ~input() = default;

        // Non-copyable but movable, required for smart pointers
        input(const input&) = delete;
        input(input&&) noexcept = default;
        input &operator=(input&&) noexcept = default;

        virtual std::vector<event_ptr> poll_events();

        virtual void cursor_mode(input_cursor_mode _mode);
        virtual input_cursor_mode cursor_mode() const;

        virtual glm::vec2 cursor_position() const;
        virtual bool key_pressed() const;
        virtual bool mouse_button_pressed() const;

    };

    using input_ptr = std::unique_ptr<input>;
    
}