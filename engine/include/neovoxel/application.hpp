#pragma once

#include <neovoxel/core.hpp>
#include <neovoxel/context.hpp>
#include <neovoxel/event.hpp>
#include <neovoxel/layer.hpp>

#include <vector>

namespace neovoxel {

    /*
        Application

        It's a singleton and it's the base class that contains everything
        needed to keep this engine running. Manages events and the layer stack.
    */
    class application : public named_resource {

    private:
        static application *_instance;

        bool _is_running;  // False only when termination is requested
        timepoint _last_tick_time;  // The last timepoint of the tick

        /* Layer Stack */
        std::vector<layer_ptr> _layer_stack;
        std::vector<layer*> _layer_insert_queue;
        std::vector<layer*> _layer_delete_queue;

        /* Context, graphics, audio */
        window_ptr _window;
        input_ptr _input;

        /* Multithreading */
        thread_pool _render_thread_pool;
        thread_pool _computation_thread_pool;

    public:
        static application &get() { return *_instance; }

        explicit application();
        explicit application(const char *_name);
        virtual ~application();

        // Singletons are non-copyable and non-movable by definition
        application(const application&) = delete;
        application(application&&) noexcept = delete;

        void run();

        virtual void push_layer(layer *_layer);
        virtual void pop_layer(layer *_layer);
        virtual void terminate();

        window &get_window() const noexcept { return *_window; }
        input &get_input() const noexcept { return *_input; }

        thread_pool &get_render_thread_pool() const noexcept { return const_cast<thread_pool&>(_render_thread_pool); }
        thread_pool &get_computation_thread_pool() const noexcept { return const_cast<thread_pool&>(_computation_thread_pool); }

    };

}
