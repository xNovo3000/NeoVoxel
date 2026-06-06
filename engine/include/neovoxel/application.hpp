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

    public:
        static application &get() { return *_instance; }

        application();
        virtual ~application();

        void run();

    };

}
