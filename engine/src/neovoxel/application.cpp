#include <pch.hpp>
#include <neovoxel/application.hpp>

namespace neovoxel {

    application *application::_instance = nullptr;

    application::application() {
        _instance = this;
    }

    application::~application() {
        _instance = nullptr;
    }
    
    void application::run() {
        
    }

}
