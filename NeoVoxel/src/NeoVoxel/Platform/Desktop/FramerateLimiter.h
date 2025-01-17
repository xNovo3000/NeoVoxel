#pragma once

#include <NeoVoxel/Core/Base.h>

#ifdef _WIN32
	#include <Windows.h>
#endif

#include <chrono>

namespace NeoVoxel {

	class FramerateLimiter {

	public:
		FramerateLimiter(uint32_t frequency);
		~FramerateLimiter();

		FramerateLimiter(const FramerateLimiter&) = delete;
		FramerateLimiter& operator=(const FramerateLimiter&) = delete;

		FramerateLimiter(FramerateLimiter&& other) noexcept;
		FramerateLimiter& operator=(FramerateLimiter&& other) noexcept;

		void setFrequency(uint32_t frequency);
		void wait();

		uint32_t getFrequency() const noexcept { return m_Frequency; }

	private:
		std::chrono::steady_clock::time_point m_LastTime;
		std::chrono::nanoseconds m_TimestepTime;
		uint32_t m_Frequency;
#ifdef _WIN32
		HANDLE m_Win32Timer;
#endif

	};

}