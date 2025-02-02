#include <PCH.h>
#include <NeoVoxel/Graphics/Material.h>

namespace NeoVoxel {

	Material::Material(const ShaderRef& shader) : m_Shader(shader) {}

	void Material::bind() {
		m_Shader->activate();
	}

}