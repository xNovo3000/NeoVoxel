#pragma once

#include <spdlog/spdlog.h>

#define NV_LOG_TRACE(...)       SPDLOG_TRACE(__VA_ARGS__)
#define NV_LOG_DEBUG(...)       SPDLOG_DEBUG(__VA_ARGS__)
#define NV_LOG_INFO(...)        SPDLOG_INFO(__VA_ARGS__)
#define NV_LOG_WARN(...)        SPDLOG_WARN(__VA_ARGS__)
#define NV_LOG_ERROR(...)       SPDLOG_ERROR(__VA_ARGS__)
#define NV_LOG_CRITICAL(...)    SPDLOG_CRITICAL(__VA_ARGS__)

namespace neovoxel::log {

    static void initialize() {
        spdlog::set_pattern("[%H:%M:%S.%F] [%^%l%$] [%t] %v");
        spdlog::set_level(spdlog::level::trace);
    }

}