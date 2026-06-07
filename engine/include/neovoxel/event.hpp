#pragma once

#include <neovoxel/core.hpp>

#include <glm/glm.hpp>

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

    class window_close_event : public event {

    public:
        static constexpr uint64_t _type = 0;

        window_close_event();

    };

    class window_focus_event : public event {

    private:
        bool _focused;

    public:
        static constexpr uint64_t _type = 1;

        window_focus_event(bool _focused);

        bool focused() const noexcept { return _focused; }

    };

    class window_size_event : public event {

    private:
        glm::ivec2 _size;

    public:
        static constexpr uint64_t _type = 2;

        window_size_event(glm::ivec2 _size);

        glm::ivec2 size() const noexcept { return _size; }

    };

    class cursor_position_event : public event {

    private:
        glm::vec2 _position;

    public:
        static constexpr uint64_t _type = 3;

        cursor_position_event(glm::vec2 _position);

        glm::vec2 position() const noexcept { return _position; }

    };

    enum class key_action { release, press, repeat };

    class key_event : public event {

    private:
        int32_t _key, _modifiers;
        key_action _action;

    public:
        static constexpr uint64_t _type = 4;

        key_event(int32_t _key, int32_t _modifiers, key_action _action);

        int32_t key() const noexcept { return _key; }
        int32_t modifiers() const noexcept { return _modifiers; }
        key_action action() const noexcept { return _action; }

    };

    enum class mouse_button_action { release, press, repeat };

    class mouse_button_event : public event {

    private:
        int32_t _button, _modifiers;
        mouse_button_action _action;

    public:
        static constexpr uint64_t _type = 5;

        mouse_button_event(int32_t _button, int32_t _modifiers, mouse_button_action _action);

        int32_t button() const noexcept { return _button; }
        int32_t modifiers() const noexcept { return _modifiers; }
        mouse_button_action action() const noexcept { return _action; }

    };

}