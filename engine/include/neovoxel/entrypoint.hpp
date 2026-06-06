#include <neovoxel/application.hpp>

extern neovoxel::application *create_application();

int main(int argc, char const *argv[]) {
    auto app = create_application();
    app->run();
    delete app;
    return 0;
}
