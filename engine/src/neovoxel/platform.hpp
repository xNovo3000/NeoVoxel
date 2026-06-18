#pragma once

#include "neovoxel/graphics.hpp"
#include <neovoxel.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace neovoxel {
    
    class base_layer : public layer,
        public event_listener<window_close_event>,
        public event_listener<window_size_event>
    {

    public:
        base_layer();

        void on_update(timestep _timestep, std::vector<event_ptr> &_events) override;
        void on_render() override;

        bool on_event(timestep _timestep, window_close_event &_event) override;
        bool on_event(timestep _timestep, window_size_event &_event) override;

    };

    struct glfw_window_spec {
        std::string _title;
        glm::ivec2 _size;
        uint32_t _refresh_rate;
    };

    using glfw_load_func = void (*(*)(const char *))();

    class glfw_window : public window {
    
    private:
        GLFWwindow *_handle;
        std::vector<event_ptr> _events;
    
    public:
        // Used to calculate the delta when the cursor is hidden (3D movement)
        glm::dvec2 _cursor_disabled_last_position;

    public:
        glfw_window(const glfw_window_spec &_spec);
        ~glfw_window() override;

        glfw_window(glfw_window &&_other) noexcept;
        glfw_window &operator=(glfw_window &&_other) noexcept;

        void push_event(event *_event);

        std::vector<event_ptr> poll_events() override;
        void swap_buffers() override;

        void set_title(const char *_title) override;
        void set_size(glm::ivec2 _size) override;
        void set_refresh_rate(uint32_t _rate) override;

        glm::ivec2 get_size() const override;
        uint32_t get_refresh_rate() const override;

        GLFWwindow *get_handle() const noexcept { return _handle; }
        glfw_load_func get_load_func() const noexcept { return glfwGetProcAddress; }

    };

    struct glfw_input_spec {
        GLFWwindow *_handle;
    };

    class glfw_input : public input {

    private:
        GLFWwindow *_handle;  // Non-holding handle, glfw_window keeps the lifecycle

    public:
        glfw_input(const glfw_input_spec &_spec);
        ~glfw_input() override = default;

        glfw_input(glfw_input &&_other) noexcept = default;
        glfw_input &operator=(glfw_input &&_other) noexcept = default;

        std::vector<event_ptr> poll_events() override;
        timepoint current_time() override;

        void set_cursor_mode(input_cursor_mode _mode) override;

        input_cursor_mode get_cursor_mode() const override;
        glm::vec2 get_cursor_position() const override;
        bool is_key_pressed(int32_t _mouse_button) const override;
        bool is_mouse_button_pressed(int32_t _mouse_button) const override;

    };

    struct opengl_graphics_api_spec {
        glfw_load_func _load_func;
    };

    class opengl_graphics_api : public graphics_api {

    public:
        explicit opengl_graphics_api(const opengl_graphics_api_spec &_spec);
        ~opengl_graphics_api() override = default;

        void clear_color(const glm::vec4 &_color) override;
        void clear_depth(double _depth) override;
        void clear_stencil(int32_t _stencil) override;

        void disable(graphics_capability _capability) override;
        void enable(graphics_capability _capability) override;

        void set_viewport(const glm::ivec2 &_size) override;

    };

}
