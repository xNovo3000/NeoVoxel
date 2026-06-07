#include <pch.hpp>
#include <neovoxel/application.hpp>

#include <neovoxel/debug.hpp>

namespace neovoxel {

    application *application::_instance = nullptr;

    application::application() : application("application") {}

    application::application(const char *_name) : _name(_name) {
        NV_LOG_INFO("Starting application '{}'", _name);
        _instance = this;
    }

    application::~application() {
        NV_LOG_INFO("Stopping application '{}'", _name);
        _instance = nullptr;
    }
    
    void application::run() {
        NV_LOG_INFO("Running application '{}'", _name);
    }

}
