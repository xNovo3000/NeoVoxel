#include <PCH.h>
#include "FramerateLimiter.h"
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	using namespace std::chrono_literals;

	/* Utilities */

	constexpr uint32_t NANOSECONDS_PER_SECOND = 1000000000;

	/* FramerateLimiter */

	FramerateLimiter::FramerateLimiter(uint32_t frequency) :
		m_LastTime(std::chrono::steady_clock::now()),
		m_TimestepTime(0ns), m_Frequency(0)
#ifdef _WIN32
		, m_Win32Timer(CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS))
#endif
	{
		setFrequency(frequency);
	}

	FramerateLimiter::~FramerateLimiter() {
#ifdef _WIN32
		CloseHandle(m_Win32Timer);
#endif
	}

	void FramerateLimiter::setFrequency(uint32_t frequency) {
		m_Frequency = frequency;
		if (frequency == 0) {
			m_TimestepTime = 0ns;
		}
		else {
			m_TimestepTime = std::chrono::nanoseconds(NANOSECONDS_PER_SECOND / frequency);
		}
	}

	void FramerateLimiter::wait() {
		auto targetTime = m_LastTime + m_TimestepTime;
		auto deltaTime = targetTime - std::chrono::steady_clock::now();
		// Sleep 1 ms less because Windows is always 1 ms off
		if (deltaTime > std::chrono::milliseconds(1)) {
#ifdef _WIN32
			if (m_Win32Timer != nullptr) {
				deltaTime = deltaTime - std::chrono::milliseconds(1);
				auto deltaTimeNanoseconds = deltaTime.count();
				LARGE_INTEGER encodedDuration = {};
				encodedDuration.QuadPart = -(deltaTimeNanoseconds / 100);
				SetWaitableTimer(m_Win32Timer, &encodedDuration, 0, NULL, NULL, FALSE);
				WaitForSingleObject(m_Win32Timer, INFINITE);
			}
			else {
				NV_ERROR("FramerateLimiter: cannot create precise timer using CreateWaitableTimerExW, frame limiting is falling back on spinlock only");
			}
#else
			std::this_thread::sleep_for(deltaTime);
#endif
		}
		// Spinlock for the last millisecond
		while (targetTime > std::chrono::steady_clock::now()) {
			std::this_thread::yield();
		}
		m_LastTime = std::chrono::steady_clock::now();
	}

}