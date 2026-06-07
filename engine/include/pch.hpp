#pragma once

#include <chrono>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>

#if defined(_WIN32)
    #include <Windows.h>
#endif

#if NV_BUILD_TYPE == NV_BUILD_TYPE_PROFILE
    #include <source_location>
#endif