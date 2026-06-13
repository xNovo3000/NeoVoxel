#include <pch.hpp>
#include <neovoxel/context.hpp>

namespace neovoxel {

    window::window() : window("window") {}
    window::window(const char *_name) : named_resource(_name) {}

    std::vector<event_ptr> window::poll_events() { return {}; }
    void window::swap_buffers() {}

    void window::set_title(const char *_title) {}
    void window::set_size(glm::ivec2 _size) {}
    void window::set_refresh_rate(uint32_t _rate) {}

    glm::ivec2 window::get_size() const { return {}; }
    uint32_t window::get_refresh_rate() const { return 0; }

    input::input() : input("input") {}
    input::input(const char *_name) : named_resource(_name) {}

    std::vector<event_ptr> input::poll_events() { return {}; }
    timepoint input::current_time() {
        auto _now = std::chrono::steady_clock::now();
        auto _nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(_now.time_since_epoch());
        return timepoint { _nanoseconds_since_epoch.count() };
    }

    void input::set_cursor_mode(input_cursor_mode _mode) {}

    input_cursor_mode input::get_cursor_mode() const { return input_cursor_mode::normal; }
    glm::vec2 input::get_cursor_position() const { return {}; }
    bool input::is_key_pressed() const { return false; }
    bool input::is_mouse_button_pressed() const { return false; }

}