#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Graphics/Shader.h>

namespace NeoVoxel {

	class Material {

	public:
		Material(const ShaderRef& shader);
		virtual ~Material() = default;

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;

		Material(Material&&) noexcept = default;
		Material& operator=(Material&&) noexcept = default;

		virtual void bind();

		const ShaderRef& getShader() const noexcept { return m_Shader; }

	protected:
		ShaderRef m_Shader;

	};

	using MaterialRef = Ref<Material>;

}