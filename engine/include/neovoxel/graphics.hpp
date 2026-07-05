#pragma once

#include <neovoxel/core.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace neovoxel {

    class gpu_buffer_data {
    
    private:
        std::vector<uint8_t> _data;

    public:
        template <typename T>
        void push(const T &_element) {
            auto _first = reinterpret_cast<uint8_t*>(&_element);
            auto _last = _first + sizeof(T);
            _data.insert(_data.end(), _first, _last);
        }
        
        const std::vector<uint8_t> &get_data() const noexcept { return _data; }

    };

    enum class gpu_buffer_draw_type { _static, dynamic };
    enum class gpu_buffer_element {
// TODO: This buffer types will be implemented later
//      vec1_i8,  vec2_i8,  vec3_i8,  vec4_i8 ,
//      vec1_u8,  vec2_u8,  vec3_u8,  vec4_u8 ,
//      vec1_i16, vec2_i16, vec3_i16, vec4_i16,
//      vec1_u16, vec2_u16, vec3_u16, vec4_u16,
        vec1_i32, vec2_i32, vec3_i32, vec4_i32,
        vec1_u32, vec2_u32, vec3_u32, vec4_u32,
        vec1_f32, vec2_f32, vec3_f32, vec4_f32
    };

    struct gpu_buffer_spec {
        std::vector<std::vector<gpu_buffer_element>> _buffers;
        gpu_buffer_draw_type _draw_type;
    };

    /*
        GPU Buffer

        This represents the current logical and physical buffers
        in the GPU memory with updating and drawing features.
        Do not use this class as-is, always use gpu_buffer_ref.
    */
    class gpu_buffer {

    private:
        std::optional<uint32_t> _handle;

    public:
        explicit gpu_buffer(const gpu_buffer_spec &_spec);
        ~gpu_buffer();

        // Non-copyable but movable, required for smart pointers
        gpu_buffer(const gpu_buffer&) = delete;
        gpu_buffer(gpu_buffer&&) noexcept = default;
        gpu_buffer &operator=(gpu_buffer&&) noexcept = default;

        void draw() const;

        // TODO: Implement raw "alloacate" function
        void set_vertex_data(uint32_t _index, const gpu_buffer_data &_data);
        void set_vertex_subdata(uint32_t _index, uint32_t _offset, const gpu_buffer_data &_data);
        template <typename T> void set_index_data(const std::vector<T> &_data);

    };

    using gpu_buffer_ref = std::shared_ptr<gpu_buffer>;

    struct gpu_shader_spec {
        std::string _vertex_shader_code;
        std::string _fragment_shader_code;
    };

    /*
        GPU Shader

        This represents a compiled shader program in the GPU memory (both vertex and fragment).
        Do not use this class as-is, always use gpu_shader_ref.
    */
    class gpu_shader {

    private:
        std::optional<uint32_t> _handle;

    public:
        explicit gpu_shader(const gpu_shader_spec &_spec);
        ~gpu_shader();

        // Non-copyable but movable, required for smart pointers
        gpu_shader(const gpu_shader&) = delete;
        gpu_shader(gpu_shader&&) noexcept = default;
        gpu_shader &operator=(gpu_shader&&) noexcept = default;

        void use() const;

        template <typename T> void set_uniform(const std::string &_name, const T &_value);

    };

    using gpu_shader_ref = std::shared_ptr<gpu_shader>;

    enum class gpu_texture_2d_usage { diffuse };

    struct gpu_texture_2d_spec {
        gpu_texture_2d_usage _usage;
        bool _generate_mipmaps;
        bool _compress;  // Unused for now
    };

    /*
        GPU Texture 2D

        This represents a 2D texture in the GPU memory.
        Do not use this class as-is, always use gpu_texture_2d_ref.
    */
    class gpu_texture_2d {

    private:
        std::optional<uint32_t> _handle;

    public:
        explicit gpu_texture_2d(const gpu_texture_2d_spec &_spec);
        ~gpu_texture_2d();

        // Non-copyable but movable, required for smart pointers
        gpu_texture_2d(const gpu_texture_2d&) = delete;
        gpu_texture_2d(gpu_texture_2d&&) noexcept = default;
        gpu_texture_2d &operator=(gpu_texture_2d&&) noexcept = default;

        void bind(uint32_t _slot = 0) const;

        void allocate(const glm::ivec2 &_size);
        void set_image_data(const glm::ivec2 &_size, const std::vector<uint8_t> &_data, uint32_t _channels);
        void set_image_subdata(const glm::ivec2 &_size, const glm::ivec2 &_offset, const std::vector<uint8_t> &_data, uint32_t _channels);

    };

    using gpu_texture_2d_ref = std::shared_ptr<gpu_texture_2d>;

#if NV_BUILD_TYPE == NV_BUILD_TYPE_DEBUG
    constexpr glm::vec4 _graphics_default_clear_color { 0.75F, 0.125F, 0.65F, 0.0F };
#else
    constexpr glm::vec4 _graphics_default_clear_color { 0.0F, 0.0F, 0.0F, 0.0F };
#endif
    constexpr float _graphics_default_clear_depth = 1.0F;
    constexpr int32_t _graphics_default_clear_stencil = 0;

    enum class graphics_capability {};

    /*
        Graphics API

        Singleton empty class that is capable of receiving instructions from `gpu_*` resources and
        update the global state of the current API in use.
    */
    class graphics_api : public named_resource {

    public:
        explicit graphics_api();
        explicit graphics_api(const char *_name);
        virtual ~graphics_api() = default;

        // Non-copyable but movable, required for smart pointers
        graphics_api(const graphics_api&) = delete;
        graphics_api(graphics_api&&) noexcept = default;
        graphics_api &operator=(graphics_api&&) noexcept = default;

        static gpu_buffer_ref create(const gpu_buffer_spec &_spec);
        static gpu_shader_ref create(const gpu_shader_spec &_spec);
        static gpu_texture_2d_ref create(const gpu_texture_2d_spec &_spec);

        virtual void clear_color(const glm::vec4 &_color = _graphics_default_clear_color);
        virtual void clear_depth(double _depth = _graphics_default_clear_depth);
        virtual void clear_stencil(int32_t _stencil = _graphics_default_clear_stencil);

        virtual void disable(graphics_capability _capability);
        virtual void enable(graphics_capability _capability);

        virtual void set_viewport(const glm::ivec2 &_size);
    
    protected:
        virtual uint32_t _gb_create(const gpu_buffer_spec &_spec);
        virtual void _gb_destroy(uint32_t _handle);
        virtual void _gb_draw(uint32_t _handle);
        virtual void _gb_set_vertex_data(uint32_t _handle, uint32_t _index, const gpu_buffer_data &_data);
        virtual void _gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const gpu_buffer_data &_data);
        virtual void _gb_set_index_data(uint32_t _handle, const std::vector<uint16_t> &_data);
        virtual void _gb_set_index_data(uint32_t _handle, const std::vector<uint32_t> &_data);
        friend class gpu_buffer;

        virtual uint32_t _gs_create(const gpu_shader_spec &_spec);
        virtual void _gs_destroy(uint32_t _handle);
        virtual void _gs_set_uniform(uint32_t _handle, const std::string &_name, const int32_t &_value);
        virtual void _gs_set_uniform(uint32_t _handle, const std::string &_name, const uint32_t &_value);
        virtual void _gs_set_uniform(uint32_t _handle, const std::string &_name, const float &_value);
        virtual void _gs_set_uniform(uint32_t _handle, const std::string &_name, const glm::mat3 &_value);
        virtual void _gs_set_uniform(uint32_t _handle, const std::string &_name, const glm::mat4 &_value);
        friend class gpu_shader;

        virtual uint32_t _gt2_create(const gpu_texture_2d_spec &_spec);
        virtual void _gt2_destroy(uint32_t _handle);
        virtual void _gt2_bind(uint32_t _handle, uint32_t _slot);
        virtual void _gt2_allocate(uint32_t _handle, const glm::ivec2 &_size);
        virtual void _gt2_set_image_data(uint32_t _handle, const glm::ivec2 &_size, const std::vector<uint8_t> &_data, uint32_t _channels);
        virtual void _gt2_set_image_subdata(uint32_t _handle, const glm::ivec2 &_size, const glm::ivec2 &_offset, const std::vector<uint8_t> &_data, uint32_t _channels);
        friend class gpu_texture_2d;

    };

    using graphics_api_ptr = std::unique_ptr<graphics_api>;

    /*
        Transform 2D

        Used to represent a transformation in the 2D space.
    */
    class transform_2d {

    private:
        glm::vec2 _position;
        float _roll, _scale;

    public:
        constexpr explicit transform_2d();
        constexpr explicit transform_2d(const glm::vec2 &_position, float _roll, float _scale);

        void set_position(const glm::vec2 &_position);
        void set_roll(float _roll);
        void set_scale(float _scale);

        const glm::vec2 &get_position() const noexcept { return _position; }
        float get_roll() const noexcept { return _roll; }
        float get_scale() const noexcept { return _scale; }

        glm::mat4 get_model_matrix() const;

    };

    /*
        Transform 3D

        Used to represent a transformation in the 3D space.
    */
    class transform_3d {

    private:
        glm::vec3 _position, _rotation;
        float _scale;

    public:
        constexpr explicit transform_3d();
        constexpr explicit transform_3d(const glm::vec3 &_position, const glm::vec3 &_rotation, float _scale);

        void set_position(const glm::vec3 &_position);
        void set_rotation(const glm::vec3 &_position);
        void set_scale(float _scale);

        const glm::vec3 &get_position() const noexcept { return _position; }
        const glm::vec3 &get_rotation() const noexcept { return _rotation; }
        float get_scale() const noexcept { return _scale; }

        glm::mat4 get_model_matrix() const;

    };

    /*
        Camera 2D

        TODO
    */
    class camera_2d {

    private:
        glm::vec2 _position;
        float _roll;

    public:
        constexpr explicit camera_2d();
        constexpr explicit camera_2d(const glm::vec2 &_position, float _roll);

        void set_position(const glm::vec2 &_position);
        void set_roll(float _roll);

        const glm::vec2 &get_position() const noexcept { return _position; }
        float get_roll() const noexcept { return _roll; }

        glm::mat4 get_view_matrix() const;

    };

    /*
        Camera 3D
    
        TODO
    */
    class camera_3d {

    private:
        glm::vec3 _position, _rotation;

    public:
        constexpr explicit camera_3d();
        constexpr explicit camera_3d(const glm::vec3 &_position, const glm::vec3 &_rotation);

        void set_position(const glm::vec3 &_position);
        void set_rotation(const glm::vec3 &_position);

        const glm::vec3 &get_position() const noexcept { return _position; }
        const glm::vec3 &get_rotation() const noexcept { return _rotation; }

        glm::mat4 get_view_matrix() const;

    };

    /*
        Projection

        Used to represent a projection for the camera
    */
    class projection {

    public:
        virtual glm::mat4 get_projection_matrix(float _aspect_ratio) const = 0;

    };

    /*
        Orthographic projection

        TODO
    */
    class orthographic_projection : public projection {

    private:
        float _zoom_percentage;
        float _min_vsize, _max_vsize;

    public:
        constexpr explicit orthographic_projection();
        constexpr explicit orthographic_projection(float _zoom_percentage);
        constexpr explicit orthographic_projection(float _zoom_percentage, float _min_vsize, float _max_vsize);

        void set_min_vsize(float _min_vsize);
        void set_max_vsize(float _max_vsize);
        void set_zoom_percentage(float _zoom);

        float get_min_vsize() const noexcept { return _min_vsize; }
        float get_max_vsize() const noexcept { return _max_vsize; }
        float get_zoom_percentage() const noexcept { return _zoom_percentage; }

        glm::mat4 get_projection_matrix(float _aspect_ratio) const override;

    };

}