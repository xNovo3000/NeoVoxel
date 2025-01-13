#pragma once

#include <NeoVoxel/Core/Base.h>

#include <spdlog/spdlog.h>

#define NV_TRACE(...)				SPDLOG_TRACE(__VA_ARGS__)
#define NV_DEBUG(...)				SPDLOG_DEBUG(__VA_ARGS__)
#define NV_INFO(...)				SPDLOG_INFO(__VA_ARGS__)
#define NV_WARN(...)				SPDLOG_WARN(__VA_ARGS__)
#define NV_ERROR(...)				SPDLOG_ERROR(__VA_ARGS__)
#define NV_CRITICAL(...)			SPDLOG_CRITICAL(__VA_ARGS__)

namespace NeoVoxel {

	namespace Log {

		static inline void initialize() {
			spdlog::set_level(spdlog::level::trace);
		}

	}

}