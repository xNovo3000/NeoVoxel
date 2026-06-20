#pragma once

#include <neovoxel/core.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace neovoxel {

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

        template <typename T> void set_vertex_data(uint32_t _index, const std::vector<T> &_data);
        template <typename T> void set_vertex_subdata(uint32_t _index, uint32_t _offset, const std::vector<T> &_data);
        template <typename T> void set_index_data(const std::vector<T> &_data);

    };

    using gpu_buffer_ref = std::shared_ptr<gpu_buffer>;

    struct gpu_shader_spec {
        std::string _vertex_shader_code;
        std::string _fragment_shader_code;
    };

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

#if NV_BUILD_TYPE == NV_BUILD_TYPE_DEBUG
    constexpr glm::vec4 _graphics_default_clear_color { 0.75F, 0.125F, 0.65F, 0.0F };
#else
    constexpr glm::vec4 _graphics_default_clear_color { 0.0F, 0.0F, 0.0F, 0.0F };
#endif
    constexpr float _graphics_default_clear_depth = 1.0F;
    constexpr int32_t _graphics_default_clear_stencil = 0;

    enum class graphics_capability {};

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
        virtual void _gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<int32_t> &_data);
        virtual void _gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<uint32_t> &_data);
        virtual void _gb_set_vertex_data(uint32_t _handle, uint32_t _index, const std::vector<float> &_data);
        virtual void _gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<int32_t> &_data);
        virtual void _gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<uint32_t> &_data);
        virtual void _gb_set_vertex_subdata(uint32_t _handle, uint32_t _index, uint32_t _offset, const std::vector<float> &_data);
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

    };

    using graphics_api_ptr = std::unique_ptr<graphics_api>;

}