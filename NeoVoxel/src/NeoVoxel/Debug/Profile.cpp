#include <PCH.h>
#include <NeoVoxel/Debug/Profile.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

#if NV_BUILD_MODE == NV_BUILD_MODE_DEBUG

	struct GpuMemoryProfilerSingleton {
		std::map<uint64_t, uint64_t> m_AllocatedMemory;
	};

	namespace GpuMemoryProfiler {

		Ptr<GpuMemoryProfilerSingleton> SINGLETON = nullptr;

		void start() {
			SINGLETON = std::make_unique<GpuMemoryProfilerSingleton>();
		}

		void stop() {
			SINGLETON.reset();
		}

		void trace(uint64_t id, uint64_t bytes) {
			if (bytes == 0) {
				SINGLETON->m_AllocatedMemory.erase(id);
			}
			else {
				SINGLETON->m_AllocatedMemory[id] = bytes;
			}
		}

		uint64_t getTotal() {
			return std::accumulate(
				SINGLETON->m_AllocatedMemory.begin(), SINGLETON->m_AllocatedMemory.end(), 0ULL,
				[](uint64_t accumulator, const std::map<uint64_t, uint64_t>::value_type& pair) {
					return accumulator + pair.second;
				}
			);
		}

	}

#elif NV_BUILD_MODE == NV_BUILD_MODE_RELEASE

	struct TraceData {
		const char* m_Name;
		std::thread::id m_ThreadId;
		bool m_IsStartEvent;
		std::chrono::steady_clock::time_point m_TimePoint;
	};

	struct ProfilerSingleton {
		ProfilerSingleton(void (*loopFunc)()) :
			m_Start(std::chrono::steady_clock::now()),
			m_OutputFile("Trace.json", std::ios::out | std::ios::trunc),
			m_Traces(),
			m_TracesLock(),
			m_IsRunning(true),
			m_Runner(loopFunc)
		{}
		std::chrono::steady_clock::time_point m_Start;
		std::fstream m_OutputFile;
		std::vector<TraceData> m_Traces;
		std::mutex m_TracesLock;
		bool m_IsRunning;
		std::thread m_Runner;
	};

	namespace Profiler {

		Ptr<ProfilerSingleton> SINGLETON = nullptr;

		static void loop() {
			// Put first character of JSON array
			SINGLETON->m_OutputFile << "[";
			// Check if traces have been written
			bool traceWritten = false;
			// Loop
			while (SINGLETON->m_IsRunning) {
				NV_INFO("Profilation in progress");
				// Wait some time
				std::this_thread::sleep_for(std::chrono::seconds(1));
				// Get accumulated traces
				std::vector<TraceData> traces = {};
				{
					std::lock_guard guard(SINGLETON->m_TracesLock);
					std::swap(SINGLETON->m_Traces, traces);
				}
				// Set traceWritten if there are traces
				if (!traces.empty()) {
					traceWritten = true;
				}
				// Print tracing data
				for (auto& data : traces) {
					SINGLETON->m_OutputFile << "{";
					SINGLETON->m_OutputFile << "\"name\":\"" << data.m_Name << "\",";
					SINGLETON->m_OutputFile << "\"cat\":\"function\",";
					SINGLETON->m_OutputFile << "\"ph\":\"" << (data.m_IsStartEvent ? 'B' : 'E') << "\",";
					auto durationFromBeginning = std::chrono::duration_cast<std::chrono::microseconds>(data.m_TimePoint - SINGLETON->m_Start).count();
					SINGLETON->m_OutputFile << "\"ts\":" << durationFromBeginning << ",";
					SINGLETON->m_OutputFile << "\"pid\":0,";
					SINGLETON->m_OutputFile << "\"tid\":" << data.m_ThreadId;
					SINGLETON->m_OutputFile << "},";
				}
			}
			// Remove last comma if there are traces and put last character of JSON array
			if (traceWritten) {
				SINGLETON->m_OutputFile.seekp(-1, std::ios::end);
			}
			SINGLETON->m_OutputFile << "]";
			// Flush output
			SINGLETON->m_OutputFile.flush();
		}

		static inline void addTrace(TraceData&& trace) {
			std::lock_guard guard(SINGLETON->m_TracesLock);
			SINGLETON->m_Traces.push_back(std::move(trace));
		}

		void start() {
			SINGLETON = std::make_unique<ProfilerSingleton>(&loop);
		}

		void stop() {
			SINGLETON->m_IsRunning = false;
			SINGLETON->m_Runner.join();
			SINGLETON.reset();
		}

	}

	ProfileFunctionWatcher::ProfileFunctionWatcher(const char* name) : m_Name(name) {
		Profiler::addTrace(TraceData{ m_Name, std::this_thread::get_id(), true, std::chrono::steady_clock::now() });
	}

	ProfileFunctionWatcher::~ProfileFunctionWatcher() {
		Profiler::addTrace(TraceData{ m_Name, std::this_thread::get_id(), false, std::chrono::steady_clock::now() });
	}

#endif

}
