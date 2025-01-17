#pragma once

#include <NeoVoxel/Core/Base.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_RELEASE

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

#if NV_BUILD_MODE == NV_BUILD_MODE_RELEASE
	#define NV_PROFILE_START				::NeoVoxel::Profiler::start()
	#define NV_PROFILE_STOP					::NeoVoxel::Profiler::stop()
	#define NV_PROFILE						::NeoVoxel::ProfileFunctionWatcher _profileFunctionWatcher__LINE__(__FUNCSIG__)
#else
	#define NV_PROFILE_START
	#define NV_PROFILE_STOP
	#define NV_PROFILE
#endif

}