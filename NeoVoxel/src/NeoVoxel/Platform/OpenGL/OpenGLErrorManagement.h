#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Debug/Log.h>

#include <glad/gl.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	static inline void glCheckError(const char* file, long line) {
		auto error = glGetError();
		if (error != GL_NO_ERROR) {
			NV_ERROR("OpenGL error {}. File: {}. Line: {}", error, file, line);
		}
	}

#endif

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG
	#define glCall(x) x; glCheckError(__FILE__, __LINE__)
#else
	#define glCall(x) x
#endif

}