#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Core/Time.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	enum class InputCursorMode { NORMAL, DISABLED };

	class Input {

	public:
		Input();
		Input(const char* debugName);
		virtual ~Input() = default;

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		Input(Input&&) noexcept = default;
		Input& operator=(Input&&) noexcept = default;

		virtual void setCursorMode(InputCursorMode cursorMode);

		virtual Timepoint getCurrentTime();

		virtual InputCursorMode getCursorMode();
		virtual glm::vec2 getCursorPosition();
		virtual bool isKeyPressed(int32_t key);
		virtual bool isMouseButtonPressed(int32_t mouseButton);

		const char* getName() const noexcept { return m_Name; }

	private:
		const char* m_Name;

	};

	using InputRef = Ref<Input>;

}