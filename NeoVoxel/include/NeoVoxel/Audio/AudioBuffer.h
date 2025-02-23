#pragma once

#include <NeoVoxel/Core/Base.h>

#include <vector>

namespace NeoVoxel {

	class AudioBuffer {

	public:
		AudioBuffer() = default;
		virtual ~AudioBuffer() = default;

		AudioBuffer(const AudioBuffer&) = delete;
		AudioBuffer& operator=(const AudioBuffer&) = delete;

		AudioBuffer(AudioBuffer&&) noexcept = default;
		AudioBuffer& operator=(AudioBuffer&&) noexcept = default;

		virtual void setData(const std::vector<int16_t>& data, int32_t frequency);

	};

	using AudioBufferRef = Ref<AudioBuffer>;

}