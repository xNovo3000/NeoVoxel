#include <pch.hpp>
#include "platform.hpp"
#include "neovoxel/graphics.hpp"

#include <neovoxel/application.hpp>
#include <neovoxel/debug.hpp>

#include <asio.hpp>

#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

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
        // Clear everyting
        application::get().get_graphics_api().clear_color();
        application::get().get_graphics_api().clear_depth();
        application::get().get_graphics_api().clear_stencil();
    }

    bool base_layer::on_event(timestep _timestep, window_close_event &_event) {
        NV_TRACING_WATCH;
        NV_LOG_DEBUG("base_layer::on_event(window_close_event): application stop requested");
        application::get().terminate();
        return false;
    }

    bool base_layer::on_event(timestep _timestep, window_size_event &_event) {
        NV_TRACING_WATCH;
        NV_LOG_DEBUG("base_layer::on_event(window_size_event): application resize requested");
        application::get().get_graphics_api().set_viewport(_event.get_size());
        return true;
    }

    /* glfw callbacks */

    constexpr glm::dvec2 _cursor_disabled_invalid(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
    constexpr glm::dvec2 _cursor_normal_position_min(-1.0, -1.0);
    constexpr glm::dvec2 _cursor_normal_position_max( 1.0,  1.0);

    static void cb_glfw_error(int _code, const char *_description) {
        NV_TRACING_WATCH;
        NV_LOG_ERROR("GLFW Error. Code: {}. Description: {}", _code, _description);
    }

    static void cb_glfw_window_close(GLFWwindow *_window) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received window close event");
        _context->push_event(new window_close_event);
    }

    static void cb_glfw_window_focus(GLFWwindow *_window, int32_t _focused) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received window focus event. Focused: {}", _focused);
        _context->push_event(new window_focus_event(_focused != 0));
    }

    // Framebuffer size instead of window size because the window is tied
    // to the logical pixels, the framebuffer instead on the physical ones
    static void cb_glfw_framebuffer_size(GLFWwindow *_window, int32_t _width, int32_t _height) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received window size event. Size: ({}, {})", _width, _height);
        _context->push_event(new window_size_event({ _width, _height }));
    }

    static void cb_glfw_key(GLFWwindow* _window, int32_t _key, int32_t _, int32_t _action, int32_t _mods) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received key event. Key: {}, action: {}, mods: {}", _key, _action, _mods);
        _context->push_event(new key_event(_key, _mods, static_cast<key_action>(_action)));
    }

    static void cb_glfw_mouse_button(GLFWwindow* _window, int32_t _button, int32_t _action, int32_t _mods) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        NV_LOG_DEBUG("GLFW: received mouse button event. Button: {}, action: {}, mods: {}", _button, _action, _mods);
        _context->push_event(new mouse_button_event(_button, _mods, static_cast<mouse_button_action>(_action)));
    }

    static void cb_glfw_cursor_pos(GLFWwindow *_window, double _xpos, double _ypos) {
        NV_TRACING_WATCH;
        auto _context = reinterpret_cast<glfw_window*>(glfwGetWindowUserPointer(_window));
        auto _cursor_input_mode = glfwGetInputMode(_window, GLFW_CURSOR);
        if (_cursor_input_mode == GLFW_CURSOR_NORMAL) {
            // Always invalidate the last position of the disabled cursor
            _context->_cursor_disabled_last_position = _cursor_disabled_invalid;
            // Map the cursor to OpenGL viewport coordinates
            glm::ivec2 _window_size;
            glfwGetWindowSize(_window, &_window_size.x, &_window_size.y);
            glm::dvec2 _cursor_position = {
                _xpos / _window_size.x * 2.0 - 1.0,
                -(_ypos / _window_size.y * 2.0 - 1.0)
            };
            _cursor_position = glm::clamp(_cursor_position,
                _cursor_normal_position_min, _cursor_normal_position_max);
            NV_LOG_TRACE("GLFW: received cursor position event. Position: ({}, {})", _cursor_position.x, _cursor_position.y);
            _context->push_event(new cursor_position_event(_cursor_position));
        } else if (_cursor_input_mode == GLFW_CURSOR_DISABLED) {
            // Map the cursor as the delta from the last one
            if (_context->_cursor_disabled_last_position == _cursor_disabled_invalid) {
                // First time, just send (0, 0)
                NV_LOG_TRACE("GLFW: received cursor position event. Position: ({}, {})", 0.0, 0.0);
                _context->push_event(new cursor_position_event({ 0.0, 0.0 }));
            } else [[likely]] {
                // Calculate delta from previous
                auto _delta = glm::dvec2 { _xpos, _ypos } - _context->_cursor_disabled_last_position;
                NV_LOG_TRACE("GLFW: received cursor position event. Position: ({}, {})", _delta.x, -_delta.y);
                _context->push_event(new cursor_position_event({ _delta.x, -_delta.y }));
            }
            // Update latest position as the current one
            _context->_cursor_disabled_last_position = { _xpos, _ypos };
        }
    }

    /* glfw_window */

    glfw_window::glfw_window(const glfw_window_spec &_spec) :
        window("glfw_window"), _handle(nullptr),
        _cursor_disabled_last_position(_cursor_disabled_invalid)
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
        // Initialize callbacks
        glfwSetWindowUserPointer(_handle, this);
        glfwSetWindowCloseCallback(_handle, cb_glfw_window_close);
        glfwSetWindowFocusCallback(_handle, cb_glfw_window_focus);
        glfwSetFramebufferSizeCallback(_handle, cb_glfw_framebuffer_size);
        glfwSetKeyCallback(_handle, cb_glfw_key);
        glfwSetMouseButtonCallback(_handle, cb_glfw_mouse_button);
        glfwSetCursorPosCallback(_handle, cb_glfw_cursor_pos);
        // Switch context off-thread
        glfwMakeContextCurrent(nullptr);
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [&]() {
                NV_TRACING_WATCH;
                glfwMakeContextCurrent(_handle);
            }
        );
    }

    glfw_window::~glfw_window() {
        NV_TRACING_WATCH;
        if (_handle != nullptr) {
            // Destroy window and terminate GLFW instance
            glfwDestroyWindow(_handle);
            glfwTerminate();
        }
    }

    glfw_window::glfw_window(glfw_window &&_other) noexcept :
        window(std::move(_other)), _handle(_other._handle),
        _cursor_disabled_last_position(_other._cursor_disabled_last_position)
    {
        _other._handle = nullptr;
        _other._cursor_disabled_last_position = _cursor_disabled_invalid;
    }

    glfw_window &glfw_window::operator=(glfw_window &&_other) noexcept {
        window::operator=(std::move(_other));
        _handle = _other._handle;
        _cursor_disabled_last_position = _other._cursor_disabled_last_position;
        _other._handle = nullptr;
        _other._cursor_disabled_last_position = _cursor_disabled_invalid;
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
        auto _swap_buffers_future = asio::post(
            application::get().get_render_thread_pool().get_executor(),
            asio::use_future([&]() {
                NV_TRACING_WATCH;
                glfwSwapBuffers(_handle);
            })
        );
        _swap_buffers_future.get();
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
                // Move cursor to window center when switching back to normal
                glm::ivec2 _window_size;
                glfwGetWindowSize(_handle, &_window_size.x, &_window_size.y);
                glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPos(_handle, _window_size.x / 2.0, _window_size.y / 2.0);
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

    /* opengl callbacks */

#if NV_BUILD_TYPE == NV_BUILD_TYPE_DEBUG
    static void cb_gl_check_error(const char *_file, long _line) {
        if (auto _error = glGetError(); _error != GL_NO_ERROR) {
            NV_LOG_ERROR("OpenGL error {}. File: {}. Line: {}", _error, _file, _line);
        }
    }
    #define glCall(x)   x; cb_gl_check_error(__FILE__, __LINE__)
#else
    #define glCall(x)   x
#endif

    /* opengl_graphics_api */

    static GLenum _util_ogl_get_draw_type(gpu_buffer_draw_type _type) {
        if (_type == gpu_buffer_draw_type::dynamic) return GL_DYNAMIC_DRAW;
        return GL_STATIC_DRAW;
    }

    static uint32_t _util_ogl_get_bytes(gpu_buffer_element _element) {
        switch (_element) {
            case gpu_buffer_element::vec1_f32: case gpu_buffer_element::vec1_i32: case gpu_buffer_element::vec1_u32: return 4;
            case gpu_buffer_element::vec2_f32: case gpu_buffer_element::vec2_i32: case gpu_buffer_element::vec2_u32: return 8;
            case gpu_buffer_element::vec3_f32: case gpu_buffer_element::vec3_i32: case gpu_buffer_element::vec3_u32: return 12;
            case gpu_buffer_element::vec4_f32: case gpu_buffer_element::vec4_i32: case gpu_buffer_element::vec4_u32: return 16;
        }
        return 0;
    }

    static GLint _util_ogl_get_elements_size(gpu_buffer_element _element) {
        switch (_element) {
            case gpu_buffer_element::vec1_f32: case gpu_buffer_element::vec1_i32: case gpu_buffer_element::vec1_u32: return 1;
            case gpu_buffer_element::vec2_f32: case gpu_buffer_element::vec2_i32: case gpu_buffer_element::vec2_u32: return 2;
            case gpu_buffer_element::vec3_f32: case gpu_buffer_element::vec3_i32: case gpu_buffer_element::vec3_u32: return 3;
            case gpu_buffer_element::vec4_f32: case gpu_buffer_element::vec4_i32: case gpu_buffer_element::vec4_u32: return 4;
        }
        return 0;
    }

    static GLenum _util_ogl_get_type(gpu_buffer_element _element) {
        switch (_element) {
            case gpu_buffer_element::vec1_f32: case gpu_buffer_element::vec2_f32: case gpu_buffer_element::vec3_f32: case gpu_buffer_element::vec4_f32: return GL_FLOAT;
            case gpu_buffer_element::vec1_i32: case gpu_buffer_element::vec2_i32: case gpu_buffer_element::vec3_i32: case gpu_buffer_element::vec4_i32: return GL_INT;
            case gpu_buffer_element::vec1_u32: case gpu_buffer_element::vec2_u32: case gpu_buffer_element::vec3_u32: case gpu_buffer_element::vec4_u32: return GL_UNSIGNED_INT;
        }
        return 0;
    }

    opengl_graphics_api::opengl_graphics_api(const opengl_graphics_api_spec &_spec) :
        graphics_api("opengl_graphics_api"),
        _gpu_buffer_data(), _gpu_buffer_data_mutex(), _gpu_buffer_data_next(0)
    {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [_spec]() {
                NV_TRACING_WATCH;
                // Initialize OpenGL
                if (gladLoadGL(_spec._load_func) != 0) {
                    NV_LOG_INFO("OpenGL: initialized");
                } else {
                    NV_LOG_ERROR("OpenGL: failed to initialize");
                }
            }
        );
    }

    void opengl_graphics_api::clear_color(const glm::vec4 &_color) {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [_color]() {
                NV_TRACING_WATCH;
                glCall(glClearColor(_color.r, _color.g, _color.b, _color.a));
                glCall(glClear(GL_COLOR_BUFFER_BIT));
            }
        );
    }

    void opengl_graphics_api::clear_depth(double _depth) {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [_depth]() {
                NV_TRACING_WATCH;
                glCall(glClearDepth(_depth));
                glCall(glClear(GL_DEPTH_BUFFER_BIT));
            }
        );
    }

    void opengl_graphics_api::clear_stencil(int32_t _stencil) {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [_stencil]() {
                NV_TRACING_WATCH;
                glCall(glClearStencil(_stencil));
                glCall(glClear(GL_STENCIL_BUFFER_BIT));
            }
        );
    }

    void opengl_graphics_api::disable(graphics_capability _capability) {}
    void opengl_graphics_api::enable(graphics_capability _capability) {}

    void opengl_graphics_api::set_viewport(const glm::ivec2 &_size) {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [_size]() {
                NV_TRACING_WATCH;
                glCall(glViewport(0, 0, _size.x, _size.y));
            }
        );
    }

    uint32_t opengl_graphics_api::_gb_create(const gpu_buffer_spec &_spec) {
        // Reserve next index available
        uint32_t _idx = _gpu_buffer_data_next;
        {
            std::lock_guard _g(_gpu_buffer_data_mutex);
            while (_gpu_buffer_data.contains(_idx))
                _idx += 1;
            _gpu_buffer_data[_idx] = {};
            _gpu_buffer_data_next = _idx + 1;
        }
        // Render thread: create the required index
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [=, this]() {
                NV_TRACING_WATCH;
                NV_LOG_DEBUG("gpu_buffer {}: creating", _idx);
                // Extract data
                opengl_gpu_buffer_data _data;
                {
                    std::lock_guard _g(_gpu_buffer_data_mutex);
                    _data = _gpu_buffer_data[_idx];
                }
                // Create VAO and set active
                glCall(glGenVertexArrays(1, &_data._vao_handle));
                glCall(glBindVertexArray(_data._vao_handle));
                // Create EBO and initialize it
                glCall(glGenBuffers(1, &_data._ebo_handle));
                glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _data._ebo_handle));
                glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, _util_ogl_get_draw_type(_spec._draw_type)));
                // Create VBOs, initialize it and set accordingly into physical/logical distinction
                glCall(glGenBuffers(_spec._buffers.size(), _data._vbo_handles.data()));
                uint32_t _physical_buffer_index = 0, _logical_buffer_index = 0;
                // Create physical VBO
                for (auto &_physical_buffer : _spec._buffers) {
                    uint32_t _physical_buffer_size = std::accumulate(
                        _physical_buffer.begin(), _physical_buffer.end(), 0u,
                        [](const uint32_t _a, const gpu_buffer_element &_element) { return _a + _util_ogl_get_bytes(_element); }
                    );
                    NV_LOG_DEBUG("gpu_buffer {}: creating physical buffer {} of {} bytes", _idx, _physical_buffer_index, _physical_buffer_size);
                    glCall(glBindBuffer(GL_ARRAY_BUFFER, _data._vbo_handles[_physical_buffer_index]));
                    glCall(glBufferData(GL_ARRAY_BUFFER, 0, nullptr, _util_ogl_get_draw_type(_spec._draw_type)));
                    // Create logical VBO
                    uint32_t _logical_buffer_offset = 0;
                    for (auto &_logical_buffer : _physical_buffer) {
                        uint32_t _logical_buffer_size = _util_ogl_get_bytes(_logical_buffer);
                        NV_LOG_DEBUG("gpu_buffer {}: creating logical buffer {} of {} bytes, offset {}", _idx, _logical_buffer_index, _logical_buffer_size, _logical_buffer_offset);
                        glCall(glVertexAttribPointer(_logical_buffer_index, _util_ogl_get_elements_size(_logical_buffer), _util_ogl_get_type(_logical_buffer), GL_FALSE, _physical_buffer_size, reinterpret_cast<void*>(_logical_buffer_offset)));
                        glCall(glEnableVertexAttribArray(_logical_buffer_index));
                        _logical_buffer_offset += _logical_buffer_size;
                        _logical_buffer_index += 1;
                    }
                    _physical_buffer_index += 1;
                }
                // Update data
                _data._vbo_handles_size = _spec._buffers.size();
                {
                    std::lock_guard _g(_gpu_buffer_data_mutex);
                    _gpu_buffer_data[_idx] = _data;
                }
            }
        );
        // Immediate: return the generated index
        return _idx;
    }

    void opengl_graphics_api::_gb_destroy(uint32_t _handle) {
        asio::post(
            application::get().get_render_thread_pool().get_executor(),
            [=, this]() {
                NV_TRACING_WATCH;
                NV_LOG_DEBUG("gpu_buffer {}: destroying", _handle);
                // Extract data
                opengl_gpu_buffer_data _data;
                {
                    std::lock_guard _g(_gpu_buffer_data_mutex);
                    _data = _gpu_buffer_data[_handle];
                }
                // Destroy all buffers
                glCall(glDeleteVertexArrays(1, &_data._vao_handle));
                glCall(glDeleteBuffers(1, &_data._ebo_handle));
                glCall(glDeleteBuffers(_data._vbo_handles_size, _data._vbo_handles.data()));
            }
        );
    }

}