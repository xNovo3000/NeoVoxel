#include <pch.hpp>
#include <neovoxel/core.hpp>

#include <neovoxel/debug.hpp>

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

    /* threading */

    static void thread_set_priority(thread_priority _priority) {
        // TODO: Implement platform-specific thread management
    }

    thread_pool::thread_pool(const char *_name, uint32_t _size, thread_priority _priority) :
        named_resource(_name),
        _context(),
        _guard(asio::make_work_guard(_context.get_executor())),
        _handles(),
        _priority(_priority)
    {
        for (uint32_t _i = 0; _i < _size; _i++) {
            _handles.emplace_back(&thread_pool::loop, this, _i);
        }
    }

    thread_pool::~thread_pool() {
        _guard.reset();
        for (auto &_handle : _handles) {
            _handle.join();
        }
    }

    void thread_pool::loop(uint32_t _index) {
        NV_LOG_INFO("Thread '{}-{}': starting", name(), _index);
        thread_set_priority(_priority);
        _context.run();
        NV_LOG_INFO("Thread '{}-{}': finishing", name(), _index);
    }

}