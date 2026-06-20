#include <pch.hpp>
#include <neovoxel/graphics.hpp>

#include <neovoxel/application.hpp>
#include <neovoxel/debug.hpp>

namespace neovoxel {

    /* gpu_buffer */

    gpu_buffer::gpu_buffer(const gpu_buffer_spec &_spec) :
        _handle(application::get().get_graphics_api().graphics_api::_gb_create(_spec))
    {}

    gpu_buffer::~gpu_buffer() {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gb_destroy(*_handle);
    }

    void gpu_buffer::draw() const {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gb_draw(*_handle);
        else
            NV_LOG_WARN("gpu_buffer:draw() called with empty handle");
    }

    template <typename T>
    void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gb_set_vertex_data(*_handle, _index, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_vertex_data() called with empty handle");
    }

    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<int32_t> &_data);
    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<uint32_t> &_data);
    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<float> &_data);

    template <typename T>
    void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gb_set_vertex_subdata(*_handle, _index, _offset, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_vertex_subdata() called with empty handle");
    }

    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<int32_t> &_data);
    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<uint32_t> &_data);
    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<float> &_data);

    template <typename T>
    void gpu_buffer::set_index_data(const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gb_set_index_data(*_handle, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_index_data() called with empty handle");
    }

    template <> void gpu_buffer::set_index_data(const std::vector<uint16_t> &_data);
    template <> void gpu_buffer::set_index_data(const std::vector<uint32_t> &_data);

    /* gpu_shader */

    gpu_shader::gpu_shader(const gpu_shader_spec &_spec) :
        _handle(application::get().get_graphics_api().graphics_api::graphics_api::_gs_create(_spec))
    {}

    gpu_shader::~gpu_shader() {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::graphics_api::_gs_destroy(*_handle);
    }

    template <typename T>
    void gpu_shader::set_uniform(const std::string &_name, const T &_value) {
        if (_handle) [[likely]]
            application::get().get_graphics_api().graphics_api::_gs_set_uniform(*_handle, _name, _value);
        else
            NV_LOG_WARN("gpu_buffer:set_index_data() called with empty handle");
    }

    template <> void gpu_shader::set_uniform(const std::string &_name, const int32_t &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const uint32_t &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const float &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const glm::mat3 &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const glm::mat4 &_value);

    /* graphics_api */

    graphics_api::graphics_api() : graphics_api("graphics_api") {}
    graphics_api::graphics_api(const char *_name) : named_resource(_name) {}

    gpu_buffer_ref graphics_api::create(const gpu_buffer_spec &_spec) {
        return std::make_shared<gpu_buffer>(_spec);
    }

    gpu_shader_ref graphics_api::create(const gpu_shader_spec &_spec) {
        return std::make_shared<gpu_shader>(_spec);
    }

    void graphics_api::clear_color(const glm::vec4 &_color) {}
    void graphics_api::clear_depth(double _depth) {}
    void graphics_api::clear_stencil(int32_t _stencil) {}

    void graphics_api::disable(graphics_capability _capability) {}
    void graphics_api::enable(graphics_capability _capability) {}

    void graphics_api::set_viewport(const glm::ivec2 &_size) {}

    uint32_t graphics_api::_gb_create(const gpu_buffer_spec &_spec) { return 0; }
    void graphics_api::_gb_destroy(uint32_t _handle) {}
    void graphics_api::_gb_draw(uint32_t _handle) {}
    void graphics_api::_gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<int32_t> &_data) {}
    void graphics_api::_gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<uint32_t> &_data) {}
    void graphics_api::_gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<float> &_data) {}
    void graphics_api::_gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<int32_t> &_data) {}
    void graphics_api::_gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<uint32_t> &_data) {}
    void graphics_api::_gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<float> &_data) {}
    void graphics_api::_gb_set_index_data(uint32_t _handle, const std::vector<uint16_t> &_data) {}
    void graphics_api::_gb_set_index_data(uint32_t _handle, const std::vector<uint32_t> &_data) {}

    uint32_t graphics_api::_gs_create(const gpu_shader_spec &_spec) { return 0; }
    void graphics_api::_gs_destroy(uint32_t _handle) {}
    void graphics_api::_gs_set_uniform(uint32_t _handle, const std::string &_name, const int32_t &_value) {}
    void graphics_api::_gs_set_uniform(uint32_t _handle, const std::string &_name, const uint32_t &_value) {}
    void graphics_api::_gs_set_uniform(uint32_t _handle, const std::string &_name, const float &_value) {}
    void graphics_api::_gs_set_uniform(uint32_t _handle, const std::string &_name, const glm::mat3 &_value) {}
    void graphics_api::_gs_set_uniform(uint32_t _handle, const std::string &_name, const glm::mat4 &_value) {}

}