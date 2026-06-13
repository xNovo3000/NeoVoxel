#include <pch.hpp>
#include <neovoxel/core.hpp>

namespace neovoxel {

    /* named_resource */

    named_resource::named_resource(const char *_name) : _name(_name) {}

    /* timestep */

    timestep::timestep(double _delta_seconds) : _delta_seconds(_delta_seconds) {}

    double timestep::delta_milliseconds() const { return _delta_seconds * 1000.0; }

    /* timepoint */
    
    timepoint::timepoint(int64_t _nanoseconds) : _nanoseconds(_nanoseconds) {}

    /* operations */

    timestep operator-(const timepoint& _lhs, const timepoint& _rhs) {
        const auto _nanoseconds_delta = _lhs.nanoseconds() - _rhs.nanoseconds();
        const auto _seconds_delta = static_cast<double>(_nanoseconds_delta) / 1000000000.0;
        return timestep(_seconds_delta);
    }

}