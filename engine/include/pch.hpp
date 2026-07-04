#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <ranges>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <asio.hpp>

#include <glm/glm.hpp>

#if defined(_WIN32)
    #include <Windows.h>
#endif