#include <pch.hpp>
#include "platform.hpp"

#include <neovoxel/debug.hpp>

namespace neovoxel {

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
        application::instance().terminate();
        return false;
    }

    bool base_layer::on_event(timestep _timestep, window_size_event &_event) {
        NV_LOG_INFO("base_layer::on_event(window_size_event): application resize requested");
        return true;
    }

}