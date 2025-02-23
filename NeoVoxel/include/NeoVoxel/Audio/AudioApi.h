#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class AudioApi {

	public:
		AudioApi();
		AudioApi(const char* name);
		virtual ~AudioApi() = default;

		AudioApi(const AudioApi&) = delete;
		AudioApi& operator=(const AudioApi&) = delete;

		AudioApi(AudioApi&&) noexcept = default;
		AudioApi& operator=(AudioApi&&) noexcept = default;

		void setListenerPosition(const glm::vec3& position);
		void setListenerOrientation(const glm::vec3& orientation);

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

	using AudioApiRef = Ref<AudioApi>;

}