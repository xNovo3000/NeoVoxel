#pragma once

#include <neovoxel.hpp>

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

}
