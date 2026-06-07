/*
    Entrypoint
    
    This must be included only one time in the client 'main.cpp' file.
    It's the scaffolding for running a neovoxel application
*/

#include <neovoxel/application.hpp>
#include <neovoxel/debug.hpp>

#if defined(_WIN32)
    #include <Windows.h>
#endif

extern neovoxel::application *create_application();

#if NV_BUILD_TYPE == NV_BUILD_TYPE_RELEASE && defined(_WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char const *argv[])
#endif
{
    neovoxel::logging::initialize();
    NV_TRACING_BEGIN();
    auto app = create_application();
    app->run();
    delete app;
    NV_TRACING_END();
    return 0;
}
