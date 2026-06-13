#include <pch.hpp>
#include "platform.hpp"

#include <neovoxel/debug.hpp>

namespace neovoxel {

    /* base_layer */

    base_layer::base_layer() : layer("base_layer") {}

    void base_layer::on_update(timestep _timestep, std::vector<event_ptr> &_events) {
        NV_TRACING_WATCH;
        layer::on_update(_timestep, _events);
        event_listener<window_close_event>::dispatch_events(_timestep, _events);
        event_listener<window_size_event>::dispatch_events(_timestep, _events);
    }

    void base_layer::on_render() {
        NV_TRACING_WATCH;
        layer::on_render();
    }

    bool base_layer::on_event(timestep _timestep, window_close_event &_event) {
        NV_TRACING_WATCH;
        NV_LOG_INFO("base_layer::on_event(window_close_event): application stop requested");
        application::get().terminate();
        return false;
    }

    bool base_layer::on_event(timestep _timestep, window_size_event &_event) {
        NV_TRACING_WATCH;
        NV_LOG_INFO("base_layer::on_event(window_size_event): application resize requested");
        // TODO: Graphics: set new viewport size
        return true;
    }

    /* glfw callbacks */

    void cb_glfw_error(int _code, const char *_description) {
        NV_TRACING_WATCH;
        NV_LOG_ERROR("GLFW Error. Code: {}. Description: {}", _code, _description);
    }

    void cb_glfw_window_close(GLFWwindow *_window) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received window close event");
        _context->push_event(new window_close_event);
    }

    /* glfw_window */

    glfw_window::glfw_window(const glfw_window_spec &_spec) :
        window("glfw_window"), _handle(nullptr)
    {
        NV_TRACING_WATCH;
        // Global GLFW error callback
        glfwSetErrorCallback(cb_glfw_error);
        // Initialize GLFW
        if (glfwInit() == GLFW_TRUE) {
            NV_LOG_INFO("GLFW: initialized");
        } else {
            NV_LOG_CRITICAL("GLFW: failed to initialize");
        }
        // Initialize window and set properties
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if !defined(NDEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
        _handle = glfwCreateWindow(_spec._size.x, _spec._size.y, _spec._title.c_str(), nullptr, nullptr);
        if (_handle != nullptr) {
            NV_LOG_INFO("GLFW: window created");
        } else {
            NV_LOG_ERROR("GLFW: failed to create window");
        }
        glfwMakeContextCurrent(_handle);
        glfwSwapInterval(_spec._refresh_rate == 0);
        // TODO: Initialize callbacks
        glfwSetWindowUserPointer(_handle, this);
        glfwSetWindowCloseCallback(_handle, cb_glfw_window_close);
    }

    glfw_window::~glfw_window() {
        if (_handle != nullptr) {
            // Destroy window and terminate GLFW instance
            glfwDestroyWindow(_handle);
            glfwTerminate();
        }
    }

    glfw_window::glfw_window(glfw_window &&_other) noexcept :
        window(std::move(_other)), _handle(_other._handle)
    {
        _other._handle = nullptr;
    }

    glfw_window &glfw_window::operator=(glfw_window &&_other) noexcept {
        window::operator=(std::move(_other));
        _handle = _other._handle;
        _other._handle = nullptr;
        return *this;
    }

    void glfw_window::push_event(event *_event) {
        NV_TRACING_WATCH;
        _events.emplace_back(_event);
    }

    std::vector<event_ptr> glfw_window::poll_events() {
        NV_TRACING_WATCH;
        glfwPollEvents();
        std::vector<event_ptr> _current_events;
        std::swap(_events, _current_events);  // Way more performant than copying entire vector
        return _current_events;
    }

    void glfw_window::swap_buffers() {
        NV_TRACING_WATCH;
        glfwSwapBuffers(_handle);
    }

    void glfw_window::set_title(const char *_title) {}
    void glfw_window::set_size(glm::ivec2 _size) {}
    void glfw_window::set_refresh_rate(uint32_t _rate) {}

    glm::ivec2 glfw_window::get_size() const { return {}; }
    uint32_t glfw_window::get_refresh_rate() const { return 0; }
    
    /* glfw_input - strictly connected with glfw_window */

    glfw_input::glfw_input(const glfw_input_spec &_spec) : input("glfw_input"), _handle(_spec._handle) {}

    std::vector<event_ptr> glfw_input::poll_events() {
        NV_TRACING_WATCH;
        return {};
    }
    
    timepoint glfw_input::current_time() {
        NV_TRACING_WATCH;
        return timepoint { static_cast<int64_t>(glfwGetTime() * 1000000000.0) };
    }

    void glfw_input::set_cursor_mode(input_cursor_mode _mode) {
        NV_TRACING_WATCH;
        switch (_mode) {
            case input_cursor_mode::normal:
                glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case input_cursor_mode::disabled:
                glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
        }
    }

    input_cursor_mode glfw_input::get_cursor_mode() const {
        NV_TRACING_WATCH;
        auto _cursor_mode = glfwGetInputMode(_handle, GLFW_CURSOR);
        switch (_cursor_mode) {
            case GLFW_CURSOR_NORMAL: return input_cursor_mode::normal;
            case GLFW_CURSOR_DISABLED: return input_cursor_mode::disabled;
        }
        return input_cursor_mode::normal;
    }

    glm::vec2 glfw_input::get_cursor_position() const {
        NV_TRACING_WATCH;
        // Map the cursor from GLFW window coordinates to OpenGL coordinates
        glm::dvec2 _cursor_position;
        glm::ivec2 _window_size;
        glfwGetCursorPos(_handle, &_cursor_position.x, &_cursor_position.y);
        glfwGetWindowSize(_handle, &_window_size.x, &_window_size.y);
        return {
            _cursor_position.x / _window_size.x * 2.0 - 1.0,
            -(_cursor_position.y / _window_size.y * 2.0 - 1.0)
        };
    }

    bool glfw_input::is_key_pressed(int32_t _key) const {
        NV_TRACING_WATCH;
        return glfwGetKey(_handle, _key) == GLFW_PRESS;
    }

    bool glfw_input::is_mouse_button_pressed(int32_t _mouse_button) const {
        NV_TRACING_WATCH;
        return glfwGetMouseButton(_handle, _mouse_button) == GLFW_PRESS;
    }

}