#pragma once

#include <NeoVoxel/Core/Base.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	namespace GpuMemoryProfiler {
		void start();
		void stop();
		void trace(uint64_t id, uint64_t bytes);
		uint64_t getTotal();
	}

#elif NV_BUILD_MODE == NV_BUILD_MODE_RELEASE

	namespace Profiler {
		void start();
		void stop();
	}

	class ProfileFunctionWatcher {
	public:
		ProfileFunctionWatcher(const char* name);
		~ProfileFunctionWatcher();
	private:
		const char* m_Name;
	};

#endif

}

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG
	#define NV_PROFILE_START				::NeoVoxel::GpuMemoryProfiler::start()
	#define NV_PROFILE_STOP					::NeoVoxel::GpuMemoryProfiler::stop()
	#define NV_PROFILE
	#define NV_PROFILE_MEMORY(x, y)			::NeoVoxel::GpuMemoryProfiler::trace(x, y)
	#define NV_PROFILE_MEMORY_GET			::NeoVoxel::GpuMemoryProfiler::getTotal()
#elif NV_BUILD_MODE == NV_BUILD_MODE_RELEASE
	#define NV_PROFILE_START				::NeoVoxel::Profiler::start()
	#define NV_PROFILE_STOP					::NeoVoxel::Profiler::stop()
	#define NV_PROFILE						::NeoVoxel::ProfileFunctionWatcher _profileFunctionWatcher__LINE__(__FUNCSIG__)
	#define NV_PROFILE_MEMORY(x, y)
	#define NV_PROFILE_MEMORY_GET
#else
	#define NV_PROFILE_START
	#define NV_PROFILE_STOP
	#define NV_PROFILE
	#define NV_PROFILE_MEMORY(x, y)
	#define NV_PROFILE_MEMORY_GET
#endif