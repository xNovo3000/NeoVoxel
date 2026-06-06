/*
    Entrypoint
    
    This must be included only one time in the client 'main.cpp' file.
    It's the scaffolding for running a neovoxel application
*/

#include <neovoxel/application.hpp>

extern neovoxel::application *create_application();

int main(int argc, char const *argv[]) {
    auto app = create_application();
    app->run();
    delete app;
    return 0;
}
