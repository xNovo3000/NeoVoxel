#pragma once

#include <NeoVoxel/Graphics/Material.h>
#include <NeoVoxel/Graphics/Texture.h>

namespace NeoVoxel {

	struct BasicTexturizedMaterialSpec {
		ShaderRef m_Shader;
		Texture2DRef m_Texture2D;
	};

	class BasicTexturizedMaterial : public Material {

	public:
		static MaterialRef create(const BasicTexturizedMaterialSpec& spec);

	public:
		BasicTexturizedMaterial(const BasicTexturizedMaterialSpec& spec);

		virtual void bind() override;

	private:
		Texture2DRef m_Texture2D;

	};

}