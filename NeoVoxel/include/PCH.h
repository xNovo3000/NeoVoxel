#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <ranges>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <glm/glm.hpp>

#ifdef _WIN32
    #include <Windows.h>
#endif  // _WIN32