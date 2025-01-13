#pragma once

#include <cstdint>
#include <memory>

namespace NeoVoxel {
	template <typename T> using Ptr = std::unique_ptr<T>;
	template <typename T> using Ref = std::shared_ptr<T>;
}