#include <neovoxel/entrypoint.hpp>

neovoxel::application *create_application() {
    return new neovoxel::application("sandbox");
}