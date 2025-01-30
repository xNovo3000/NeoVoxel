#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	namespace GpuMemoryProfiler {
		void start();
		void stop();
		void trace(uint64_t id, uint64_t bytes);
		uint64_t getTotal();
	}

	#define NV_PROFILE_START				::NeoVoxel::GpuMemoryProfiler::start()
	#define NV_PROFILE_STOP					::NeoVoxel::GpuMemoryProfiler::stop()
	#define NV_PROFILE
	#define NV_PROFILE_MEMORY(id, bytes)	::NeoVoxel::GpuMemoryProfiler::trace(id, bytes)
	#define NV_PROFILE_MEMORY_PRINT			NV_TRACE("GpuMemoryProfiler: {} MBs used", static_cast<float>(::NeoVoxel::GpuMemoryProfiler::getTotal()) / (1024.0F * 1024.0F))

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

	#define NV_PROFILE_START				::NeoVoxel::Profiler::start()
	#define NV_PROFILE_STOP					::NeoVoxel::Profiler::stop()
	#define NV_PROFILE						::NeoVoxel::ProfileFunctionWatcher _profileFunctionWatcher__LINE__(__FUNCSIG__)
	#define NV_PROFILE_MEMORY(id, bytes)
	#define NV_PROFILE_MEMORY_PRINT

#else
	#define NV_PROFILE_START
	#define NV_PROFILE_STOP
	#define NV_PROFILE
	#define NV_PROFILE_MEMORY(id, bytes)
	#define NV_PROFILE_MEMORY_PRINT
#endif

}