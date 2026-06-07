#pragma once

#include <cstdint>

namespace neovoxel {

    /*
        Timestep

        Represents the time elapsed from one frame to another
    */
    class timestep {
    
    private:
        double _delta_seconds;

    public:
        explicit timestep(double _delta_seconds);

        double delta_seconds() const noexcept { return _delta_seconds; }
        double delta_milliseconds() const;

    };

    /*
        Timepoint

        Represents the absolute time elapsed from application startup
    */
    class timepoint {

    private:
        int64_t _nanoseconds;

    public:
        explicit timepoint(int64_t _nanoseconds);

        int64_t nanoseconds() const noexcept { return _nanoseconds; }

    };

    timestep operator-(const timepoint& _lhs, const timepoint& _rhs);

}