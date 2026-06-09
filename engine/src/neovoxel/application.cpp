#include <pch.hpp>
#include <neovoxel/application.hpp>

#include <neovoxel/debug.hpp>

namespace neovoxel {

    application *application::_instance = nullptr;

    application::application() : application("application") {}

    application::application(const char *_name) : named_resource(_name),
        _is_running(true), _last_tick_time(0),
        _layer_stack(), _layer_insert_queue(), _layer_delete_queue(),
        _window(nullptr), _input(nullptr)
    {
        NV_LOG_INFO("Application '{}': created", name());
        _instance = this;
        // Create singleton instances
        _window = std::make_unique<window>();
        _input = std::make_unique<input>();
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
            auto current_tick_time = _input->current_time();
            auto timestep = current_tick_time - _last_tick_time;
            _last_tick_time = current_tick_time;

            // Extract events for this loop
            auto window_events = _window->poll_events();
            auto input_events = _input->poll_events();
            std::vector<event_ptr> events;
            events.reserve(window_events.size() + input_events.size());
            events.insert(events.end(), std::make_move_iterator(window_events.begin()), std::make_move_iterator(window_events.end()));
            events.insert(events.end(), std::make_move_iterator(input_events.begin()), std::make_move_iterator(input_events.end()));

            // Print timestep and number of events
            NV_LOG_TRACE("Timestep: {} ms. Events: {}", timestep.delta_milliseconds(), events.size());

            // Update (reverse order)
            for (auto &layer : _layer_stack | std::views::reverse) {
                layer->on_update(timestep, events);
                // Remove events that should not be propagated down
                std::erase_if(events, [](const event_ptr &event) { return !event->propagate(); });
            }

            // Render (forward order)
            for (auto &layer : _layer_stack) {
                layer->on_render();
            }

            // Insert layers
            for (auto layer_to_insert : _layer_insert_queue) {
                layer_to_insert->on_create();
                if (!_layer_stack.empty()) {
                    _layer_stack.back()->on_cover();
                }
                layer_to_insert->on_visible();
                _layer_stack.emplace_back(layer_to_insert);
            }
            _layer_insert_queue.clear();

            // Delete layers
            for (auto layer_to_delete : _layer_delete_queue) {
                // Find layer
                auto maybe_found = std::ranges::find_if(_layer_stack,
                    [layer_to_delete](const layer_ptr &layer) { return layer.get() == layer_to_delete; });
                if (maybe_found == _layer_stack.end()) {
                    NV_LOG_WARN("Layer {} not found in the stack", layer_to_delete->name());
                    continue;
                }
                auto &found = *maybe_found;
                // Make lower layer visible and cover this if it's the top layer
                if (maybe_found == _layer_stack.end() - 1) {
                    found->on_cover();
                    if (!_layer_stack.empty()) {
                        _layer_stack.back()->on_visible();
                    }
                }
                // Delete this layer
                found->on_destroy();
                _layer_stack.erase(maybe_found);
            }
            _layer_delete_queue.clear();

            // Update window subsystem
            _window->swap_buffers();

        }

        NV_LOG_INFO("Application '{}': stopped", name());
    }

    void application::terminate() {
        NV_LOG_INFO("Application '{}': termination requested");
        _is_running = false;
    }

}
