/*
    Application

    It's a singleton and it's the base class that contains everything
    needed to keep this engine running. Manages events and the layer stack.
*/

#pragma once

namespace neovoxel {

    class application {

    private:
        static application *_instance;

        const char *_name;

    public:
        static application &get() { return *_instance; }

        application();
        application(const char *_name);
        virtual ~application();

        void run();

        const char *name() const noexcept { return _name; }

    };

}
