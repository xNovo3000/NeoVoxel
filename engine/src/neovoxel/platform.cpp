#include <pch.hpp>
#include "platform.hpp"

#include <neovoxel/debug.hpp>

namespace neovoxel {

    /* base_layer */

    base_layer::base_layer() : layer("base_layer") {}

    void base_layer::on_update(timestep _timestep, std::vector<event_ptr> &_events) {
        layer::on_update(_timestep, _events);
        event_listener<window_close_event>::dispatch_events(_timestep, _events);
        event_listener<window_size_event>::dispatch_events(_timestep, _events);
    }

    void base_layer::on_render() {
        layer::on_render();
    }

    bool base_layer::on_event(timestep _timestep, window_close_event &_event) {
        NV_LOG_INFO("base_layer::on_event(window_close_event): application stop requested");
        application::get().terminate();
        return false;
    }

    bool base_layer::on_event(timestep _timestep, window_size_event &_event) {
        NV_LOG_INFO("base_layer::on_event(window_size_event): application resize requested");
        // TODO: Graphics: set new viewport size
        return true;
    }

    /* glfw_window */

    glfw_window::glfw_window(const glfw_window_spec &_spec) : window("glfw_window") {}
    glfw_window::~glfw_window() {}

    glfw_window::glfw_window(glfw_window &&_other) noexcept : window(std::move(_other)) {}
    glfw_window &glfw_window::operator=(glfw_window &&_other) noexcept {
        window::operator=(std::move(_other));
        return *this;
    }

    std::vector<event_ptr> glfw_window::poll_events() { return {}; }
    void glfw_window::swap_buffers() {}

    void glfw_window::set_title(const char *_title) {}
    void glfw_window::set_size(glm::ivec2 _size) {}
    void glfw_window::set_refresh_rate(uint32_t _rate) {}

    glm::ivec2 glfw_window::get_size() const { return {}; }
    uint32_t glfw_window::get_refresh_rate() const { return 0; }
    
    /* glfw_input - strictly connected with glfw_window */

    glfw_input::glfw_input(const glfw_input_spec &_spec) : input("glfw_input") {}
    glfw_input::~glfw_input() {}

    glfw_input::glfw_input(glfw_input &&_other) noexcept : input(std::move(_other)) {}
    glfw_input &glfw_input::operator=(glfw_input &&_other) noexcept {
        input::operator=(std::move(_other));
        return *this;
    }

    std::vector<event_ptr> glfw_input::poll_events() {}
    timepoint glfw_input::current_time() {}

    void glfw_input::set_cursor_mode(input_cursor_mode _mode) {}

    input_cursor_mode glfw_input::get_cursor_mode() const { return input_cursor_mode::normal; }
    glm::vec2 glfw_input::get_cursor_position() const { return {}; }
    bool glfw_input::is_key_pressed() const { return false; }
    bool glfw_input::is_mouse_button_pressed() const { return false; }

}