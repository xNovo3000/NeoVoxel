#pragma once

#include <chrono>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

#include <glm/glm.hpp>

#ifdef _WIN32
    #include <Windows.h>
#endif  // _WIN32