#include <pch.hpp>
#include <neovoxel/graphics.hpp>

#include <neovoxel/application.hpp>
#include <neovoxel/debug.hpp>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace neovoxel {

    /* gpu_buffer */

    gpu_buffer::gpu_buffer(const gpu_buffer_spec &_spec) :
        _handle(application::get().get_graphics_api()._gb_create(_spec))
    {}

    gpu_buffer::~gpu_buffer() {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gb_destroy(*_handle);
    }

    void gpu_buffer::draw() const {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gb_draw(*_handle);
        else
            NV_LOG_WARN("gpu_buffer:draw() called with empty handle");
    }

    template <typename T>
    void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gb_set_vertex_data(*_handle, _index, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_vertex_data() called with empty handle");
    }

    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<int32_t> &_data);
    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<uint32_t> &_data);
    template <> void gpu_buffer::set_vertex_data(uint32_t _index, const std::vector<float> &_data);

    template <typename T>
    void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gb_set_vertex_subdata(*_handle, _index, _offset, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_vertex_subdata() called with empty handle");
    }

    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<int32_t> &_data);
    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<uint32_t> &_data);
    template <> void gpu_buffer::set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<float> &_data);

    template <typename T>
    void gpu_buffer::set_index_data(const std::vector<T> &_data) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gb_set_index_data(*_handle, _data);
        else
            NV_LOG_WARN("gpu_buffer:set_index_data() called with empty handle");
    }

    template <> void gpu_buffer::set_index_data(const std::vector<uint16_t> &_data);
    template <> void gpu_buffer::set_index_data(const std::vector<uint32_t> &_data);

    /* gpu_shader */

    gpu_shader::gpu_shader(const gpu_shader_spec &_spec) :
        _handle(application::get().get_graphics_api()._gs_create(_spec))
    {}

    gpu_shader::~gpu_shader() {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gs_destroy(*_handle);
    }

    template <typename T>
    void gpu_shader::set_uniform(const std::string &_name, const T &_value) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gs_set_uniform(*_handle, _name, _value);
        else
            NV_LOG_WARN("gpu_buffer:set_uniform() called with empty handle");
    }

    template <> void gpu_shader::set_uniform(const std::string &_name, const int32_t &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const uint32_t &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const float &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const glm::mat3 &_value);
    template <> void gpu_shader::set_uniform(const std::string &_name, const glm::mat4 &_value);

    /* gpu_texture_2d */

    gpu_texture_2d::gpu_texture_2d(const gpu_texture_2d_spec &_spec) :
        _handle(application::get().get_graphics_api()._gt2_create(_spec))
    {}

    gpu_texture_2d::~gpu_texture_2d() {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gt2_destroy(*_handle);
    }

    void gpu_texture_2d::bind(uint32_t _slot) const {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gt2_bind(*_handle, _slot);
        else
            NV_LOG_WARN("gpu_buffer:bind() called with empty handle");
    }

    void gpu_texture_2d::allocate(const glm::ivec2 &_size) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gt2_allocate(*_handle, _size);
        else
            NV_LOG_WARN("gpu_buffer:allocate() called with empty handle");
    }

    void gpu_texture_2d::set_image_data(const glm::ivec2 &_size, const std::vector<uint8_t> &_data, uint32_t _channels) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gt2_set_image_data(*_handle, _size, _data, _channels);
        else
            NV_LOG_WARN("gpu_buffer:set_image_data() called with empty handle");
    }

    void gpu_texture_2d::set_image_subdata(const glm::ivec2 &_size, const glm::ivec2 &_offset, const std::vector<uint8_t> &_data, uint32_t _channels) {
        if (_handle) [[likely]]
            application::get().get_graphics_api()._gt2_set_image_subdata(*_handle, _size, _offset, _data, _channels);
        else
            NV_LOG_WARN("gpu_buffer:set_image_subdata() called with empty handle");
    }

    /* graphics_api */

    graphics_api::graphics_api() : graphics_api("graphics_api") {}
    graphics_api::graphics_api(const char *_name) : named_resource(_name) {}

    gpu_buffer_ref graphics_api::create(const gpu_buffer_spec &_spec) {
        return std::make_shared<gpu_buffer>(_spec);
    }

    gpu_shader_ref graphics_api::create(const gpu_shader_spec &_spec) {
        return std::make_shared<gpu_shader>(_spec);
    }

    gpu_texture_2d_ref graphics_api::create(const gpu_texture_2d_spec &_spec) {
        return std::make_shared<gpu_texture_2d>(_spec);
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

    uint32_t graphics_api::_gt2_create(const gpu_texture_2d_spec &_spec) { return 0; }
    void graphics_api::_gt2_destroy(uint32_t _handle) {}
    void graphics_api::_gt2_bind(uint32_t _handle, uint32_t _slot) {}
    void graphics_api::_gt2_allocate(uint32_t _handle, const glm::ivec2 &_size) {}
    void graphics_api::_gt2_set_image_data(uint32_t _handle, const glm::ivec2 &_size, const std::vector<uint8_t> &_data, uint32_t _channels) {}
    void graphics_api::_gt2_set_image_subdata(uint32_t _handle, const glm::ivec2 &_size, const glm::ivec2 &_offset, const std::vector<uint8_t> &_data, uint32_t _channels) {}

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

    /*
    TODO: Zoom fix from Perplexity

    Short answer: use an exponential mapping for the zoom parameter (or equivalently use the logarithm for the UI control) so equal UI steps change the view scale multiplicatively rather than additively — e.g. set scale = exp(k * zoomInput) and compute your ortho extents from scale. This makes zooming in and out symmetric and perceptually uniform.

    Explanation and practical formulas

    What’s happening

    Your code treats _zoom as a linear denominator so a fixed additive change in _zoom produces very different multiplicative changes in the view size depending on the current value (small _zoom → large fractional change, large _zoom → small fractional change). This yields fast zoom-in and slow zoom-out behavior.

    Use an exponential (log) mapping

    Let s be the scale that controls the half-extent of your orthographic view (what you currently compute as _vertical = 1 / _zoom). Rather than using s = 1 / _zoom, use:
    s = s0 * exp(k * z)
    where z is the user-controlled zoom parameter, s0 is a base scale (for z = 0), and k controls sensitivity (e.g., k = ln(2) makes one unit in z double/halve the scale).

    Compute extents from s:
    vertical = s
    horizontal = s * aspect
    projection = glm::ortho(-horizontal, horizontal, -vertical, vertical)

    Common convenient choices

    If you want one mouse-wheel “tick” to double/halve the view size: choose k = ln(2). Then z += 1 doubles s (zooms out), z -= 1 halves s (zooms in).

    If you prefer smaller steps use k = ln(1.1) so each tick scales by 1.1.

    If you have a UI slider in range [0..1], map it to a larger z range linearly (e.g., z = (slider - 0.5) * 10) before applying exp to get useful dynamic range.

    Concrete replacement for your function

    Pick s0 = 1.0 (or whatever "neutral" half-height you want) and a sensitivity k. Example with k = ln(2):

    glm::mat4 orthographic_projection::get_projection_matrix(float _aspect_ratio) const {
    float s0 = 1.0f; // base half-extent at zoom = 0
    float k = 0.69314718f; // ln(2) -> each unit of _zoom doubles the extent
    float scale = s0 * exp(k * _zoom); // exponential mapping
    float vertical = scale;
    float horizontal = vertical * _aspect_ratio;
    return glm::ortho(-horizontal, horizontal, -vertical, vertical);
    }

    If in your code _zoom currently increases to zoom in, invert sign: use exp(-k * _zoom) so larger _zoom makes the view smaller.

    Why exponential/log fixes it

    Perceptually, zoom feels multiplicative: each step should scale the view by a constant factor, not add a constant amount to extents. The exponential mapping makes fixed input steps produce constant multiplicative changes, so zoom-in and zoom-out speeds feel symmetric.

    Extra tips

    Clamp scale to avoid extremely small/large extents (e.g., scale in [1e-4, 1e4]).

    Internally keep a log-scale zoom value (logScale). Update logScale += delta when the user scrolls; compute scale = exp(logScale) for the projection. This avoids numerical issues and makes it easy to implement smooth interpolation.

    If you want mouse-centered zoom (zoom toward cursor), adjust the camera position when changing scale so the world point under the cursor remains fixed.

    Would you like a version where the slider/mouse-wheel maps to zoom in a particular way (e.g., one tick = 1.1×) or an example that keeps zoom centered on the cursor?


    */

}