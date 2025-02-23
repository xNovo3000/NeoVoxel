#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Audio/AudioBuffer.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class AudioSource {

	public:
		virtual ~AudioSource() = default;

		AudioSource(const AudioSource&) = delete;
		AudioSource& operator=(const AudioSource&) = delete;

		AudioSource(AudioSource&&) noexcept = default;
		AudioSource& operator=(AudioSource&&) noexcept = default;

		virtual void bind(const AudioBuffer& buffer);
		virtual void unbind();

		virtual void setPosition(const glm::vec3& position);
		virtual void setOrientation(const glm::vec3& orientation);

		virtual void play();
		virtual void pause();
		virtual void stop();

	};

	using AudioSourceRef = Ref<AudioSource>;

}