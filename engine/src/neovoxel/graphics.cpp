#include <pch.hpp>
#include <neovoxel/graphics.hpp>

#include <neovoxel/application.hpp>
#include <neovoxel/debug.hpp>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    /* transform_2d */

    constexpr transform_2d::transform_2d() : transform_2d({}, 0.0F, 1.0F) {}
    constexpr transform_2d::transform_2d(const glm::vec2 &_position, float _roll, float _scale) :
        _position(_position), _roll(_roll), _scale(_scale)
    {}

    void transform_2d::set_position(const glm::vec2 &_position) {
        this->_position = _position;
    }

    void transform_2d::set_roll(float _roll) {
        this->_roll = _roll;
        while (this->_roll > glm::pi<float>()) [[unlikely]] {
            this->_roll -= glm::two_pi<float>();
        }
        while (this->_roll <= -glm::pi<float>()) [[unlikely]] {
            this->_roll += glm::two_pi<float>();
        }
    }

    void transform_2d::set_scale(float _scale) {
        this->_scale = glm::max(_scale, glm::epsilon<float>());
    }

    glm::mat4 transform_2d::get_model_matrix() const {
        glm::mat4 _result;
        _result = glm::scale(_result, glm::vec3(_scale));
        _result = glm::rotate(_result, _roll, glm::vec3(0.0F, 0.0F, 1.0F));
        _result = glm::translate(_result, glm::vec3(_position, 0.0F));
        return _result;
    }

    /* transform_3d */

    constexpr transform_3d::transform_3d() : transform_3d({}, {}, 1.0F) {}
    constexpr transform_3d::transform_3d(const glm::vec3 &_position, const glm::vec3 &_rotation, float _scale) :
        _position(_position), _rotation(_rotation), _scale(_scale)
    {}

    void transform_3d::set_position(const glm::vec3 &_position) {
        this->_position = _position;
    }

    void transform_3d::set_rotation(const glm::vec3 &_rotation) {
        this->_rotation = _rotation;
        while (this->_rotation.x > glm::pi<float>()) [[unlikely]] {
            this->_rotation.x -= glm::two_pi<float>();
        }
        while (this->_rotation.x <= -glm::pi<float>()) [[unlikely]] {
            this->_rotation.x += glm::two_pi<float>();
        }
        while (this->_rotation.y > glm::pi<float>()) [[unlikely]] {
            this->_rotation.y -= glm::two_pi<float>();
        }
        while (this->_rotation.y <= -glm::pi<float>()) [[unlikely]] {
            this->_rotation.y += glm::two_pi<float>();
        }
        while (this->_rotation.z > glm::pi<float>()) [[unlikely]] {
            this->_rotation.z -= glm::two_pi<float>();
        }
        while (this->_rotation.z <= -glm::pi<float>()) [[unlikely]] {
            this->_rotation.z += glm::two_pi<float>();
        }
    }

    void transform_3d::set_scale(float _scale) {
        this->_scale = glm::max(_scale, glm::epsilon<float>());
    }

    glm::mat4 transform_3d::get_model_matrix() const {
        glm::mat4 _result;
        _result = glm::scale(_result, glm::vec3(_scale));
        _result = glm::rotate(_result, _rotation.z, glm::vec3(0.0F, 0.0F, 1.0F));
        _result = glm::rotate(_result, _rotation.y, glm::vec3(0.0F, 1.0F, 0.0F));
        _result = glm::rotate(_result, _rotation.x, glm::vec3(1.0F, 0.0F, 0.0F));
        _result = glm::translate(_result, _position);
        return _result;
    }

    /* orthographic_projection */

    constexpr orthographic_projection::orthographic_projection() : orthographic_projection(1.0F) {}
    constexpr orthographic_projection::orthographic_projection(float _zoom) : _zoom(_zoom) {}

    void orthographic_projection::set_zoom(float _zoom) {
        this->_zoom = glm::max(_zoom, glm::epsilon<float>());
    }

    glm::mat4 orthographic_projection::get_projection_matrix(float _aspect_ratio) const {
        float _vertical = 1 / _zoom;
        float _horizontal = _vertical * _aspect_ratio;
        return glm::ortho(-_horizontal, _horizontal, -_vertical, _vertical);
    }

}