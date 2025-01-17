#include <PCH.h>
#include "GlfwWindow.h"
#include <NeoVoxel/Debug/Log.h>

#include <NeoVoxel/Core/Event/CursorPositionEvent.h>
#include <NeoVoxel/Core/Event/KeyEvent.h>
#include <NeoVoxel/Core/Event/MouseButtonEvent.h>
#include <NeoVoxel/Core/Event/WindowCloseEvent.h>
#include <NeoVoxel/Core/Event/WindowFocusEvent.h>
#include <NeoVoxel/Core/Event/WindowSizeEvent.h>

namespace NeoVoxel {

	/* Callback - Error */

	static void callback_GlfwError(int32_t errorCode, const char* errorDescription) {
		NV_CRITICAL("GLFW error. Code: {}. Description: '{}'", errorCode, errorDescription);
	}

	/* GlfwWindow */

	GlfwWindow::GlfwWindow(const GlfwWindowSpec& spec) : Window("GlfwWindow"),
		m_WindowHandle(nullptr), m_EventList(), m_FramerateLimiter(spec.m_Framerate)
	{
		// Set GLFW error callback
		glfwSetErrorCallback(callback_GlfwError);
		// Init GLFW
		if (glfwInit() != GLFW_TRUE) {
			NV_CRITICAL("{}: GLFW initialization error", getName());
		}
		// Create window and make thread current
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if NV_BUILD_MODE < NV_BUILD_MODE_DIST
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
#if defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
		m_WindowHandle = glfwCreateWindow(spec.m_Size.x, spec.m_Size.y, spec.m_Title.c_str(), nullptr, nullptr);
		if (m_WindowHandle == nullptr) {
			NV_CRITICAL("{}: failed to create GLFW window", getName());
		}
		glfwMakeContextCurrent(m_WindowHandle);
		setRefreshRate(spec.m_Framerate);
		// Set pointer for callbacks
		glfwSetWindowUserPointer(m_WindowHandle, this);
		// TODO: Set callbacks
		// Move context to render thread
		glfwMakeContextCurrent(nullptr);
	}

	GlfwWindow::~GlfwWindow() {
		if (m_WindowHandle != nullptr) {
			glfwDestroyWindow(m_WindowHandle);
			glfwTerminate();
		}
	}

	GlfwWindow::GlfwWindow(GlfwWindow&& other) noexcept : Window(std::move(other)),
		m_WindowHandle(other.m_WindowHandle),
		m_EventList(std::move(other.m_EventList)),
		m_FramerateLimiter(std::move(other.m_FramerateLimiter))
	{
		other.m_WindowHandle = nullptr;
	}

	GlfwWindow& GlfwWindow::operator=(GlfwWindow&& other) noexcept {
		Window::operator=(std::move(other));
		m_WindowHandle = other.m_WindowHandle;
		m_EventList = std::move(other.m_EventList);
		m_FramerateLimiter = std::move(other.m_FramerateLimiter);
		other.m_WindowHandle = nullptr;
		return *this;
	}

	std::vector<EventPtr> GlfwWindow::pollEvents() {
		std::vector<EventPtr> other;
		glfwPollEvents();
		std::swap(m_EventList, other);
		return other;
	}

	void GlfwWindow::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
		m_FramerateLimiter.wait();
	}

	void GlfwWindow::setRefreshRate(uint32_t refreshRate) {
		m_FramerateLimiter.setFrequency(refreshRate);
	}

	void GlfwWindow::setSize(const glm::ivec2& size) {
		glfwSetWindowSize(m_WindowHandle, size.x, size.y);
	}

	void GlfwWindow::setTitle(const std::string& title) {
		glfwSetWindowTitle(m_WindowHandle, title.c_str());
	}

	uint32_t GlfwWindow::getRefreshRate() {
		return m_FramerateLimiter.getFrequency();
	}

	glm::ivec2 GlfwWindow::getSize() {
		glm::ivec2 size{};
		glfwGetWindowSize(m_WindowHandle, &size.x, &size.y);
		return size;
	}

}