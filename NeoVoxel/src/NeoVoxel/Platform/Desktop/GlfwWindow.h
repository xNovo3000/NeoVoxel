#pragma once

#include <NeoVoxel/Core/Window.h>
#include <NeoVoxel/Core/Event.h>
#include "FramerateLimiter.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace NeoVoxel {

	struct GlfwWindowSpec {
		std::string m_Title;
		glm::ivec2 m_Size;
		uint32_t m_Framerate;
	};

	using ContextLoaderPtr = void (*(*)(const char*))(void);

	class GlfwWindow : public Window {

	public:
		GlfwWindow(const GlfwWindowSpec& spec);
		virtual ~GlfwWindow();

		GlfwWindow(const GlfwWindow&) = delete;
		GlfwWindow& operator=(const GlfwWindow&) = delete;

		GlfwWindow(GlfwWindow&& other) noexcept;
		GlfwWindow& operator=(GlfwWindow&& other) noexcept;

		virtual std::vector<EventPtr> pollEvents() override;
		virtual void swapBuffers() override;

		virtual void setRefreshRate(uint32_t refreshRate) override;
		virtual void setSize(const glm::ivec2& size) override;
		virtual void setTitle(const std::string& title) override;

		virtual uint32_t getRefreshRate() override;
		virtual glm::ivec2 getSize() override;

		void pushEvent(Event* event);

		ContextLoaderPtr getContextLoaderPtr() { return glfwGetProcAddress; }
		GLFWwindow* getHandle() const noexcept { return m_WindowHandle; }

	private:
		GLFWwindow* m_WindowHandle;
		std::vector<EventPtr> m_EventList;
		FramerateLimiter m_FramerateLimiter;

	public:
		glm::dvec2 m_LastCursorPosition;

	};

}