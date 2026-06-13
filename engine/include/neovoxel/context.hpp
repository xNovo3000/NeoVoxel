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

        virtual void set_title(const char *_title);
        virtual void set_size(glm::ivec2 _size);
        virtual void set_refresh_rate(uint32_t _rate);

        virtual glm::ivec2 get_size() const;
        virtual uint32_t get_refresh_rate() const;

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
        virtual timepoint current_time();

        virtual void set_cursor_mode(input_cursor_mode _mode);

        virtual input_cursor_mode get_cursor_mode() const;
        virtual glm::vec2 get_cursor_position() const;
        virtual bool is_key_pressed() const;
        virtual bool is_mouse_button_pressed() const;

    };

    using input_ptr = std::unique_ptr<input>;
    
}