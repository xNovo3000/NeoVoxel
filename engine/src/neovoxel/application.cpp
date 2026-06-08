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
    }

    application::~application() {
        NV_LOG_INFO("Application '{}': destroyed", name());
        _instance = nullptr;
    }
    
    void application::run() {
        NV_LOG_INFO("Application '{}': started", name());
        while (_is_running) {
            NV_TRACING_WATCH;
        }
        NV_LOG_INFO("Application '{}': stopped", name());
    }

    void application::terminate() {
        NV_LOG_INFO("Application '{}': termination requested");
        _is_running = false;
    }

}
