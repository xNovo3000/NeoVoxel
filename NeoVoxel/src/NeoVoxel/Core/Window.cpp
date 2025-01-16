#include <PCH.h>
#include <NeoVoxel/Core/Window.h>

namespace NeoVoxel {

	Window::Window() : m_Name("Window") {}
	Window::Window(const char* name) : m_Name(name) {}

	std::vector<EventPtr> Window::pollEvents() {
		return std::vector<EventPtr>();
	}

	void Window::swapBuffers() {}

	void Window::setRefreshRate(uint32_t refreshRate) {}
	void Window::setSize(const glm::ivec2& size) {}
	void Window::setTitle(const std::string& title) {}

	uint32_t Window::getRefreshRate() {
		return 0;
	}

	glm::ivec2 Window::getSize() {
		return glm::ivec2();
	}

}