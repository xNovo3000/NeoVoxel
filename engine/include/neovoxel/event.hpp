#pragma once

#include <neovoxel/core.hpp>

#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

namespace neovoxel {

    /*
        Event

        Represents any event the the application wants to pass to the layerstack.
        Can be a window, input or a custom event. The best way to get events in a layer
        is by using the event_listener class for that specific event.
        Every subclass must expose a unique static uint64_t variable called _type.
    */
    class event {

    private:
        uint64_t _propagate : 1;
        uint64_t _type : 63;

    public:
        explicit event(uint64_t _type);
        virtual ~event() = default;

        // Non-copyable but movable, required for smart pointers
        event(const event&) = delete;
        event(event&&) noexcept = default;
        event &operator=(event&&) noexcept = default;

        void stop_propagation();

        bool propagate() const noexcept { return _propagate == 1; }
        uint64_t type() const noexcept { return _type; }

    };

    using event_ptr = std::unique_ptr<event>;

    /*
        Event Listener

        Used in the layerstack to listen for a specific event type without
        checking for the type explicitly every time.
    */
    template <typename _e>
    class event_listener {

        static_assert(std::is_base_of_v<event, _e>, "_e does not inherit from Event");

    public:
        virtual ~event_listener() = default;

        virtual void dispatch_events(timestep _timestep, std::vector<event_ptr> &_events) {
            for (auto &_event : _events) {
                if (_event->type() == _e::_type) {
                    bool _continue_propagation = on_event(_timestep, static_cast<_e&>(*_event));
                    if (!_continue_propagation) {
                        _event->stop_propagation();
                    }
                }
            }
        }

        // True if event should be propagated to a lower layer
        virtual bool on_event(timestep _timestep, _e &_event) = 0;

    };

}