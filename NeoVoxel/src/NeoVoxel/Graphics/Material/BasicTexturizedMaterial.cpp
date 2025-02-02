#include <PCH.h>
#include <NeoVoxel/Graphics/Material/BasicTexturizedMaterial.h>

namespace NeoVoxel {

	MaterialRef BasicTexturizedMaterial::create(const BasicTexturizedMaterialSpec& spec) {
		return std::make_shared<BasicTexturizedMaterial>(spec);
	}

	BasicTexturizedMaterial::BasicTexturizedMaterial(const BasicTexturizedMaterialSpec& spec) :
		Material(spec.m_Shader), m_Texture2D(spec.m_Texture2D)
	{}

	void BasicTexturizedMaterial::bind() {
		Material::bind();
		m_Texture2D->bind();
		m_Shader->setUniform(2, 0);
	}

}