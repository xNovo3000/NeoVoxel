/*
    Debug

    Here all the debug-related features for neovoxel, like logging and tracing
*/

#pragma once

#include <spdlog/spdlog.h>  // Include only one time in this header

#if NV_BUILD_TYPE == NV_BUILD_TYPE_DEBUG
    #undef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_ACTIVE_LEVEL_DEBUG
#elif NV_BUILD_TYPE == NV_BUILD_TYPE_PROFILE
    #undef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_ACTIVE_LEVEL_INFO
#else
    #undef SPDLOG_ACTIVE_LEVEL
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_ACTIVE_LEVEL_ERROR
#endif

#define NV_LOG_TRACE(...)       SPDLOG_TRACE(__VA_ARGS__)       // Logged every frame
#define NV_LOG_DEBUG(...)       SPDLOG_DEBUG(__VA_ARGS__)       // Logged once something happens
#define NV_LOG_INFO(...)        SPDLOG_INFO(__VA_ARGS__)        // Logged for the user to see
#define NV_LOG_WARN(...)        SPDLOG_WARN(__VA_ARGS__)        // Logged to tell something wrong happened but it's recoverable
#define NV_LOG_ERROR(...)       SPDLOG_ERROR(__VA_ARGS__)       // Logged to tell something is not working
#define NV_LOG_CRITICAL(...)    SPDLOG_CRITICAL(__VA_ARGS__)    // Logged to tell app is stopping

namespace neovoxel {

    namespace logging {

        static void initialize() {
            spdlog::set_pattern("[%H:%M:%S.%F] [%^%l%$] [%t] %v");
            spdlog::set_level(spdlog::level::debug);
        }

    }

#if NV_BUILD_TYPE == NV_BUILD_TYPE_PROFILE

    namespace tracing {

        void start();
        void stop();

        class function_watcher {
        
        private:
            const char *_name;

        public:
            function_watcher(const char *_name);
            ~function_watcher();

        };

    }

    #define NV_TRACING_BEGIN    ::neovoxel::tracing::start()
    #define NV_TRACING_END      ::neovoxel::tracing::stop()
    #define NV_TRACING_WATCH    ::neovoxel::tracing::function_watcher __watcher(__func__);

#else
    // Provide empty macros by default
    #define NV_TRACING_BEGIN
    #define NV_TRACING_END
    #define NV_TRACING_WATCH
#endif

}