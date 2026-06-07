#pragma once

#include <neovoxel/core.hpp>

namespace neovoxel {

    /*
        Application

        It's a singleton and it's the base class that contains everything
        needed to keep this engine running. Manages events and the layer stack.
    */
    class application : public named_resource {

    private:
        static application *_instance;

    public:
        static application &instance() { return *_instance; }

        explicit application();
        explicit application(const char *_name);
        virtual ~application();

        // Singletons are non-copyable and non-movable by definition
        application(const application&) = delete;
        application(application&&) noexcept = delete;

        void run();

    };

}
