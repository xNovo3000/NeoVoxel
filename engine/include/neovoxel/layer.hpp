#pragma once

#include <neovoxel/core.hpp>
#include <neovoxel/event.hpp>

#include <memory>
#include <vector>

namespace neovoxel {

    /*
        Layer

        Represents a layer in the application. Lifecycle:
        - on_create: called when added to the layerstack (do not use constructor)
        - on_visible: called when it's the first layer in the stack
        - on_cover: called when it's no more the first layer in the stack
        - on_destroy: called when it's removed from the layersack (do not use destructor)
        - on_update: called every tick (top to bottom)
        - on_render: called every tick (bottom to top)
        Since only one layer at a time can be the current layer, the function
        'current' returns the current active layer in any lifecycle call
    */
    class layer : public named_resource {

    private:
        static layer *_current;

        bool _front;
    
    protected:
        // True if this layer is on the top of the stack
        bool is_front() const noexcept { return _front; }

    public:
        static layer &current() { return *_current; }

        explicit layer();
        explicit layer(const char *_name);
        virtual ~layer() = default;

        // Non-copyable but movable, required for smart pointers
        layer(const layer&) = delete;
        layer(layer&&) noexcept = default;
        layer &operator=(layer&&) noexcept = default;

        virtual void on_create();
        virtual void on_visible();
        virtual void on_cover();
        virtual void on_destroy();

        virtual void on_update(timestep _timestep, std::vector<event_ptr> &_events);
        virtual void on_render();

    };

    using layer_ptr = std::unique_ptr<layer>;

}