#include <pch.hpp>
#include <neovoxel/layer.hpp>

#include <neovoxel/debug.hpp>

namespace neovoxel {

    layer *layer::_current = nullptr;

    layer::layer() : layer("layer") {}
    layer::layer(const char *_name) : named_resource(_name), _front(false) {}

    void layer::on_create() {
        _current = this;
        NV_LOG_DEBUG("Layer '{}': on_create called", name());
    }

    void layer::on_visible() {
        _current = this;
        _front = true;
        NV_LOG_DEBUG("Layer '{}': on_visible called", name());
    }

    void layer::on_cover() {
        _current = this;
        _front = false;
        NV_LOG_DEBUG("Layer '{}': on_cover called", name());
    }

    void layer::on_destroy() {
        _current = this;
        NV_LOG_DEBUG("Layer '{}': on_destroy called", name());
    }

    void layer::on_update(timestep _timestep, std::vector<event_ptr> &_events) {
        _current = this;
        NV_LOG_TRACE("Layer '{}': on_update called", name());
    }

    void layer::on_render() {
        _current = this;
        NV_LOG_TRACE("Layer '{}': on_render called", name());
    }

}