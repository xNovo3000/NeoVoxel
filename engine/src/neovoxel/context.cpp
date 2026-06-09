#include <pch.hpp>
#include <neovoxel/context.hpp>

namespace neovoxel {

    window::window() : window("window") {}
    window::window(const char *_name) : named_resource(_name) {}

    std::vector<event_ptr> window::poll_events() { return {}; }
    void window::swap_buffers() {}

    void window::title(const char *_title) {}

    void window::size(glm::ivec2 _size) {}
    glm::ivec2 window::size() const { return {}; }

    void window::refresh_rate(uint32_t _rate) {}
    uint32_t window::refresh_rate() const { return 0; }

    input::input() : input("input") {}
    input::input(const char *_name) : named_resource(_name) {}

    std::vector<event_ptr> input::poll_events() { return {}; }

    timepoint input::current_time() {
        auto now = std::chrono::steady_clock::now();
        auto nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        return timepoint { nanoseconds_since_epoch.count() };
    }

    void input::cursor_mode(input_cursor_mode _mode) {}
    input_cursor_mode input::cursor_mode() const { return input_cursor_mode::normal; }

    glm::vec2 input::cursor_position() const { return {}; }
    bool input::key_pressed() const { return false; }
    bool input::mouse_button_pressed() const { return false; }

}