#pragma once

#include <NeoVoxel/Core/Base.h>

namespace NeoVoxel {

	class Application {

	private:
		static Application* INSTANCE;

	public:
		static inline Application& get() noexcept { return *INSTANCE; }

	public:
		Application();
		Application(const char* name);
		virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		Application(Application&&) noexcept = delete;
		Application& operator=(Application&&) noexcept = delete;

		void run();

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

}