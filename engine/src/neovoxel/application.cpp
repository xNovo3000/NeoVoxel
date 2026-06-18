#include <memory>
#include <pch.hpp>
#include <neovoxel/application.hpp>

#include <neovoxel/debug.hpp>

#include "neovoxel/graphics.hpp"
#include "platform.hpp"

namespace neovoxel {

    application *application::_instance = nullptr;

    application::application() : application("application") {}

    application::application(const char *_name) : named_resource(_name),
        _is_running(true), _last_tick_time(0),
        _layer_stack(), _layer_insert_queue(), _layer_delete_queue(),
        _window(nullptr), _input(nullptr), _graphics_api(nullptr),
        _render_thread_pool("render", 1, thread_priority::high),
        _computation_thread_pool("computation", std::thread::hardware_concurrency(), thread_priority::low)
    {
        NV_TRACING_WATCH;
        NV_LOG_INFO("Application '{}': created", name());
        _instance = this;
        // Create window
        glfw_window_spec _window_spec {
            ._title = "NeoVoxel",
            ._size = { 960, 540 },
            ._refresh_rate = 0
        };
        glfw_window *_window_ptr = new glfw_window(_window_spec);
        _window = std::unique_ptr<glfw_window>(_window_ptr);
        // Create input
        glfw_input_spec _input_spec {
            ._handle = _window_ptr->get_handle()
        };
        glfw_input *_input_ptr = new glfw_input(_input_spec);
        _input = std::unique_ptr<glfw_input>(_input_ptr);
        // Create graphics
        opengl_graphics_api_spec _graphics_api_spec {
            ._load_func = _window_ptr->get_load_func()
        };
        opengl_graphics_api *_graphics_api_ptr = new opengl_graphics_api(_graphics_api_spec);
        _graphics_api = std::unique_ptr<opengl_graphics_api>(_graphics_api_ptr);
        // Create base layer
        push_layer(new base_layer);
    }

    application::~application() {
        NV_LOG_INFO("Application '{}': destroyed", name());
        _instance = nullptr;
    }
    
    void application::run() {
        NV_LOG_INFO("Application '{}': started", name());

        // Main loop
        while (_is_running) {
            NV_TRACING_WATCH;

            // Extract current timestep
            auto _current_tick_time = _input->current_time();
            auto _timestep = _current_tick_time - _last_tick_time;
            _last_tick_time = _current_tick_time;

            // Extract events for this loop
            auto _window_events = _window->poll_events();
            auto _input_events = _input->poll_events();
            std::vector<event_ptr> _events;
            _events.reserve(_window_events.size() + _input_events.size());
            _events.insert(_events.end(), std::make_move_iterator(_window_events.begin()), std::make_move_iterator(_window_events.end()));
            _events.insert(_events.end(), std::make_move_iterator(_input_events.begin()), std::make_move_iterator(_input_events.end()));

            // Print timestep and number of events
            NV_LOG_TRACE("Timestep: {} ms. Events: {}", _timestep.delta_milliseconds(), _events.size());

            // Update (reverse order)
            for (auto &_layer : _layer_stack | std::views::reverse) {
                _layer->on_update(_timestep, _events);
                // Remove events that should not be propagated down
                std::erase_if(_events, [](const event_ptr &event) { return !event->should_propagate(); });
            }

            // Render (forward order)
            for (auto &_layer : _layer_stack) {
                _layer->on_render();
            }

            // Insert layers
            for (auto _layer_to_insert : _layer_insert_queue) {
                _layer_to_insert->on_create();
                if (!_layer_stack.empty()) {
                    _layer_stack.back()->on_cover();
                }
                _layer_to_insert->on_visible();
                _layer_stack.emplace_back(_layer_to_insert);
            }
            _layer_insert_queue.clear();

            // Delete layers
            for (auto _layer_to_delete : _layer_delete_queue) {
                // Find layer
                auto _maybe_found = std::ranges::find_if(_layer_stack,
                    [_layer_to_delete](const layer_ptr &_layer) { return _layer.get() == _layer_to_delete; });
                if (_maybe_found == _layer_stack.end()) {
                    // Not found
                    NV_LOG_WARN("Layer {} not found in the stack", _layer_to_delete->name());
                    continue;
                } else if (_maybe_found == _layer_stack.end() - 1) {
                    // Last layer in the stack
                    (*_maybe_found)->on_cover();
                    (*_maybe_found)->on_destroy();
                    _layer_stack.erase(_maybe_found);
                    if (!_layer_stack.empty()) {
                        _layer_stack.back()->on_visible();
                    }
                } else {
                    // Not last layer in the stack
                    (*_maybe_found)->on_destroy();
                    _layer_stack.erase(_maybe_found);
                }
            }
            _layer_delete_queue.clear();

            // Update window subsystem
            _window->swap_buffers();

        }

        NV_LOG_INFO("Application '{}': stopped", name());
    }
    
    void application::push_layer(layer *_layer) {
        if (_is_running) {
            NV_LOG_DEBUG("Application '{}': requested to insert layer {}", name(), fmt::ptr(_layer));
            _layer_insert_queue.emplace_back(_layer);
        }
    }

    void application::pop_layer(layer *_layer) {
        NV_LOG_DEBUG("Application '{}': requested to pop layer {}", name(), fmt::ptr(_layer));
        _layer_delete_queue.emplace_back(_layer);
    }

    void application::terminate() {
        NV_LOG_INFO("Application '{}': termination requested", name());
        _is_running = false;
        for (auto &_layer : _layer_stack) {
            pop_layer(_layer.get());
        }
    }

}
