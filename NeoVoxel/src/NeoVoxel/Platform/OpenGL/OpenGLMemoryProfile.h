#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Debug/Profile.h>

#include <glad/gl.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG
	#define NV_PROFILE_MEMORY_ARRAY_BUFFER(handle, index, bytes)	NV_PROFILE_MEMORY((0ULL << 63) | ((uint64_t) index << 59) | ((uint64_t) handle), bytes)
	#define NV_PROFILE_MEMORY_TEXTURE_2D(handle, bytes)				NV_PROFILE_MEMORY((0ULL << 63) |							((uint64_t) handle), bytes)
#else
	#define NV_PROFILE_MEMORY_ARRAY_BUFFER(handle, index, bytes)
	#define NV_PROFILE_MEMORY_TEXTURE_2D(handle, bytes)
#endif

}