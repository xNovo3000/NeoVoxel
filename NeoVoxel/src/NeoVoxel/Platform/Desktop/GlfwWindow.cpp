#include <PCH.h>
#include "GlfwWindow.h"
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#include <NeoVoxel/Core/Event/CursorPositionEvent.h>
#include <NeoVoxel/Core/Event/KeyEvent.h>
#include <NeoVoxel/Core/Event/MouseButtonEvent.h>
#include <NeoVoxel/Core/Event/WindowCloseEvent.h>
#include <NeoVoxel/Core/Event/WindowFocusEvent.h>
#include <NeoVoxel/Core/Event/WindowSizeEvent.h>

namespace NeoVoxel {

	/* Utility */

	constexpr glm::dvec2 INVALID_MOUSE = glm::dvec2(std::numeric_limits<float>::max());

	/* Callback - Error */

	static void callback_GlfwError(int32_t errorCode, const char* errorDescription) {
		NV_CRITICAL("GLFW error. Code: {}. Description: '{}'", errorCode, errorDescription);
	}

	/* Callbacks */

	static void callback_WindowClose(GLFWwindow* window) {
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		context->pushEvent(new WindowCloseEvent());
	}

	static void callback_WindowSize(GLFWwindow* window, int32_t width, int32_t height) {
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		context->pushEvent(new WindowSizeEvent({ width, height }));
	}

	static void callback_WindowFocus(GLFWwindow* window, int32_t focused) {
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		context->pushEvent(new WindowFocusEvent(focused == GLFW_TRUE));
	}

	static void callback_Key(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
		auto keyAction = KeyAction::PRESS;
		switch (action) {
			case GLFW_PRESS: keyAction = KeyAction::PRESS; break;
			case GLFW_RELEASE: keyAction = KeyAction::RELEASE; break;
			case GLFW_REPEAT: keyAction = KeyAction::REPEAT; break;
		}
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		context->pushEvent(new KeyEvent(key, keyAction, mods));
	}

	static void callback_MouseButton(GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
		auto mouseAction = MouseButtonAction::PRESS;
		switch (action) {
			case GLFW_PRESS: mouseAction = MouseButtonAction::PRESS; break;
			case GLFW_RELEASE: mouseAction = MouseButtonAction::RELEASE; break;
			case GLFW_REPEAT: mouseAction = MouseButtonAction::REPEAT; break;
		}
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		context->pushEvent(new MouseButtonEvent(button, mouseAction, mods));
	}

	static void callback_CursorPosition(GLFWwindow* window, double xpos, double ypos) {
		auto context = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		// Check for current cursor mode
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			auto cursorPosition = glm::dvec2(xpos, ypos);
			// 3D camera movement, must return the delta
			if (context->m_LastCursorPosition == INVALID_MOUSE) {
				// First movement doesn't count
				context->pushEvent(new CursorPositionEvent({ 0.0F, 0.0F }));
			}
			else {
				// Push the delta position
				auto delta = cursorPosition - context->m_LastCursorPosition;
				context->pushEvent(new CursorPositionEvent({ delta.x, -delta.y }));
			}
			// Set last position
			context->m_LastCursorPosition = cursorPosition;
		}
		else {
			// Invalidate last position for 3d camera
			context->m_LastCursorPosition = INVALID_MOUSE;
			// Normal mouse movement, must follow OpenGL coordinate system
			auto windowSize = glm::ivec2();
			glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
			auto cursorPosition = glm::vec2();
			cursorPosition.x = static_cast<float>(xpos) / windowSize.x * 2.0F - 1.0F;
			cursorPosition.y = -(static_cast<float>(ypos) / windowSize.y * 2.0F - 1.0F);
			context->pushEvent(new CursorPositionEvent(cursorPosition));
		}
	}

	/* GlfwWindow */

	GlfwWindow::GlfwWindow(const GlfwWindowSpec& spec) : Window("GlfwWindow"),
		m_WindowHandle(nullptr), m_EventList(),
		m_FramerateLimiter(spec.m_Framerate), m_LastCursorPosition(INVALID_MOUSE)
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
		// Set callbacks
		glfwSetCursorPosCallback(m_WindowHandle, callback_CursorPosition);
		glfwSetMouseButtonCallback(m_WindowHandle, callback_MouseButton);
		glfwSetKeyCallback(m_WindowHandle, callback_Key);
		glfwSetWindowFocusCallback(m_WindowHandle, callback_WindowFocus);
		glfwSetWindowCloseCallback(m_WindowHandle, callback_WindowClose);
		glfwSetWindowSizeCallback(m_WindowHandle, callback_WindowSize);
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
		m_FramerateLimiter(std::move(other.m_FramerateLimiter)),
		m_LastCursorPosition(other.m_LastCursorPosition)
	{
		other.m_WindowHandle = nullptr;
	}

	GlfwWindow& GlfwWindow::operator=(GlfwWindow&& other) noexcept {
		Window::operator=(std::move(other));
		m_WindowHandle = other.m_WindowHandle;
		m_EventList = std::move(other.m_EventList);
		m_FramerateLimiter = std::move(other.m_FramerateLimiter);
		m_LastCursorPosition = other.m_LastCursorPosition;
		other.m_WindowHandle = nullptr;
		return *this;
	}

	std::vector<EventPtr> GlfwWindow::pollEvents() {
		NV_PROFILE;
		std::vector<EventPtr> other;
		glfwPollEvents();
		std::swap(m_EventList, other);
		return other;
	}

	void GlfwWindow::swapBuffers() {
		NV_PROFILE;
		glfwSwapBuffers(m_WindowHandle);
		m_FramerateLimiter.wait();
	}

	void GlfwWindow::setRefreshRate(uint32_t refreshRate) {
		glfwSwapInterval(refreshRate == 0);
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

	void GlfwWindow::pushEvent(Event* event) {
		m_EventList.emplace_back(event);
	}

}