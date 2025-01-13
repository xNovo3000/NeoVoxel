#include <PCH.h>
#include <NeoVoxel/Application.h>

namespace NeoVoxel {

	/* Application - Singleton */

	Application* Application::INSTANCE = nullptr;

	/* Application */

	Application::Application() : Application("Application") {}
	Application::Application(const char* name) : m_Name(name) {}
	Application::~Application() {}

	void Application::run() {
		// TODO: Loop
	}

}