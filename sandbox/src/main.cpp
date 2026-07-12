#include <neovoxel/entrypoint.hpp>

class my_layer : public neovoxel::layer,
    public neovoxel::event_listener<neovoxel::key_event>,
    public neovoxel::event_listener<neovoxel::mouse_button_event>
{

private:
    neovoxel::gpu_buffer_ref _ref;

public:
    my_layer() : layer("my_layer") {}

    void on_create() {
        layer::on_create();
        auto gpu_buffer_spec = neovoxel::gpu_buffer_spec {
            ._buffers = {
                {
                    neovoxel::gpu_buffer_element::vec3_f32,
                    neovoxel::gpu_buffer_element::vec2_f32,
                    neovoxel::gpu_buffer_element::vec3_f32
                },
                {
                    neovoxel::gpu_buffer_element::vec3_f32
                }
            },
            ._draw_type = neovoxel::gpu_buffer_draw_type::_static
        };
        _ref = neovoxel::graphics_api::create(gpu_buffer_spec);
    }

    void on_update(neovoxel::timestep _timestep, std::vector<neovoxel::event_ptr> &_events) {
        layer::on_update(_timestep, _events);
        neovoxel::event_listener<neovoxel::key_event>::dispatch_events(_timestep, _events);
        neovoxel::event_listener<neovoxel::mouse_button_event>::dispatch_events(_timestep, _events);
    }

    void on_render() {
        layer::on_render();
        _ref->draw();
    }

    bool on_event(neovoxel::timestep _timestep, neovoxel::key_event &_event) {
        // Set cursor normal when clicking escape
        if (_event.get_key() == NV_KEY_ESCAPE && _event.get_action() == neovoxel::key_action::press) {
            neovoxel::application::get().get_input().set_cursor_mode(neovoxel::input_cursor_mode::normal);
        }
        return true;
    }

    bool on_event(neovoxel::timestep _timestep, neovoxel::mouse_button_event &_event) {
        // Set cursor disabled when left clicking in the window
        if (_event.get_button() == NV_MOUSE_BUTTON_LEFT && _event.get_action() == neovoxel::mouse_button_action::press) {
            neovoxel::application::get().get_input().set_cursor_mode(neovoxel::input_cursor_mode::disabled);
        }
        return true;
    }

};

class my_application : public neovoxel::application {

public:
    my_application() : application("my_application") {
        push_layer(new my_layer);
    }

};

neovoxel::application *create_application() {
    return new my_application;
}
