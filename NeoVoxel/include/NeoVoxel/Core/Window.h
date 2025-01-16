#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Core/Event.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace NeoVoxel {

	class Window {

	public:
		Window();
		Window(const char* name);
		virtual ~Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) noexcept = default;
		Window& operator=(Window&&) noexcept = default;

		virtual std::vector<EventPtr> pollEvents();
		virtual void swapBuffers();

		virtual void setRefreshRate(uint32_t refreshRate);
		virtual void setSize(const glm::ivec2& size);
		virtual void setTitle(const std::string& title);

		virtual uint32_t getRefreshRate();
		virtual glm::ivec2 getSize();

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

	using WindowRef = Ref<Window>;

}