#include <PCH.h>
#include "GlfwInput.h"

namespace NeoVoxel {

	GlfwInput::GlfwInput(GLFWwindow* window) : Input("GlfwInput"), m_Window(window) {}

	Timepoint GlfwInput::getCurrentTime() {
		return Timepoint(static_cast<int64_t>(glfwGetTime() * 1000000000.0));
	}

	void GlfwInput::setCursorMode(InputCursorMode cursorMode) {
		auto oldCursorMode = glfwGetInputMode(m_Window, GLFW_CURSOR);
		switch (cursorMode) {
			case NeoVoxel::InputCursorMode::NORMAL: glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
			case NeoVoxel::InputCursorMode::DISABLED: glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
		}
		if (oldCursorMode == GLFW_CURSOR_DISABLED && cursorMode == InputCursorMode::NORMAL) {
			// Center cursor when becoming visible again
			auto windowSize = glm::ivec2();
			glfwGetWindowSize(m_Window, &windowSize.x, &windowSize.y);
			glfwSetCursorPos(m_Window, windowSize.x / 2.0, windowSize.y / 2.0);
		}
	}

	InputCursorMode GlfwInput::getCursorMode() {
		switch (glfwGetInputMode(m_Window, GLFW_CURSOR)) {
			case GLFW_CURSOR_NORMAL: return InputCursorMode::NORMAL;
			case GLFW_CURSOR_HIDDEN: return InputCursorMode::DISABLED;
		}
		return InputCursorMode::NORMAL;
	}

	glm::vec2 GlfwInput::getCursorPosition() {
		// Get cursor position
		auto cursorPosition = glm::dvec2();
		glfwGetCursorPos(m_Window, &cursorPosition.x, &cursorPosition.y);
		// Normal mouse movement, must follow OpenGL coordinate system
		auto windowSize = glm::ivec2();
		glfwGetWindowSize(m_Window, &windowSize.x, &windowSize.y);
		return { cursorPosition.x / windowSize.x * 2.0 - 1.0, -(cursorPosition.y / windowSize.y * 2.0 - 1.0) };
	}

	bool GlfwInput::isKeyPressed(int32_t key) {
		return glfwGetKey(m_Window, key) == GLFW_PRESS;
	}

	bool GlfwInput::isMouseButtonPressed(int32_t mouseButton) {
		return glfwGetMouseButton(m_Window, mouseButton) == GLFW_PRESS;
	}

}