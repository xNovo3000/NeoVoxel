#include <pch.hpp>
#include <neovoxel/debug.hpp>

namespace neovoxel {

#if NV_BUILD_TYPE == NV_BUILD_TYPE_PROFILE

    namespace tracing {

        /* tracer singleton */

        struct data {
            const char *_name;
            std::thread::id _thread_id;
            bool _is_start_event;
            std::chrono::steady_clock::time_point _time_point;
        };

        struct tracer {
            std::chrono::steady_clock::time_point _start_time;
            std::fstream _output_file;
            std::vector<data> _traces;
            std::mutex _lock;
            std::thread _runner;
            bool _is_running;
        };

        tracer *_singleton = nullptr;

        static void loop() {
            using namespace std::chrono_literals;
            // TODO: Set current thread priority to low
            // This function does not use a JSON parser, but pushes all traces
            // in sequence removing the trailing comma and adding the closing
            // square bracket at the end.
            _singleton->_output_file << "[";
            while (_singleton->_is_running)
            {
                NV_LOG_DEBUG("Tracer: running");
                // Better performance than copying the container
                std::vector<data> _traces;
                {
                    std::lock_guard _guard(_singleton->_lock);
                    std::swap(_singleton->_traces, _traces);
                }
                // Print traces
                for (auto& [m_Name, m_ThreadId, m_IsStartEvent, m_TimePoint] : _traces)
                {
                    _singleton->_output_file << "{";
					_singleton->_output_file << R"("name":")" << m_Name << "\",";
					_singleton->_output_file << R"("cat":"function",)";
					_singleton->_output_file << R"("ph":")" << (m_IsStartEvent ? 'B' : 'E') << "\",";
					const auto durationFromBeginning = std::chrono::duration_cast<std::chrono::microseconds>(m_TimePoint - _singleton->_start_time).count();
					_singleton->_output_file << "\"ts\":" << durationFromBeginning << ",";
					_singleton->_output_file << "\"pid\":0,";
#if defined(__APPLE__)
                    // On macOS the thread id is just a pointer to the thread handle (fucking morons...)
                    // So, we need to get a number and the hashing function is a perfect candidate
                    _singleton->_output_file << "\"tid\":" << std::hash<std::thread::id>()(m_ThreadId);
#else
                    _singleton->_output_file << "\"tid\":" << m_ThreadId;
#endif
					_singleton->_output_file << "},";
                }
                // Wait some time until next loop
                std::this_thread::sleep_for(1s);
            }
            _singleton->_output_file.seekp(-1, std::ios::end);  // Remove trailing comma
            _singleton->_output_file << "]";
        }

        void start() {
            NV_LOG_INFO("Tracer: starting");
            _singleton = new tracer {
                std::chrono::steady_clock::now(),
                std::fstream("Traces.json", std::ios::out | std::ios::trunc),
                std::vector<data>(),
                std::mutex(),
                std::thread(&loop),
                true
            };
        }

        void stop() {
            NV_LOG_INFO("Tracer: terminating");
            _singleton->_is_running = false;
            _singleton->_runner.join();
            delete _singleton;
            _singleton = nullptr;
        }

        static void add_trace(data &&_data) {
            std::lock_guard _guard(_singleton->_lock);
            _singleton->_traces.emplace_back(std::move(_data));
        }

        /* function_watcher */

        function_watcher::function_watcher(const char *_name) : _name(_name) {
            auto _data = data { _name, std::this_thread::get_id(), true, std::chrono::steady_clock::now() };
            add_trace(std::move(_data));
        }

        function_watcher::~function_watcher() {
            auto _data = data { _name, std::this_thread::get_id(), false, std::chrono::steady_clock::now() };
            add_trace(std::move(_data));
        }

    }

#endif

}