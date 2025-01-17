#pragma once

#include <NeoVoxel/Core/Input.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace NeoVoxel {

	class GlfwInput : public Input {

	public:
		GlfwInput(GLFWwindow* window);
		virtual ~GlfwInput() = default;

		GlfwInput(const GlfwInput&) = delete;
		GlfwInput& operator=(const GlfwInput&) = delete;

		GlfwInput(GlfwInput&&) noexcept = default;
		GlfwInput& operator=(GlfwInput&&) noexcept = default;

		virtual Timepoint getCurrentTime() override;

		virtual void setCursorMode(InputCursorMode cursorMode) override;

		virtual InputCursorMode getCursorMode() override;
		virtual glm::vec2 getCursorPosition() override;
		virtual bool isKeyPressed(int32_t key) override;
		virtual bool isMouseButtonPressed(int32_t mouseButton) override;

	private:
		GLFWwindow* m_Window;

	};

}