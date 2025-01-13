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
		~Application();

		void run();

	protected:
		const char* m_Name;

	};

}