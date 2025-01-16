#include <PCH.h>
#include <NeoVoxel/Core/Input.h>

namespace NeoVoxel {

	Input::Input() : m_Name("Input") {}
	Input::Input(const char* debugName) : m_Name(debugName) {}

	void Input::setCursorMode(InputCursorMode cursorMode) {}

	Timepoint Input::getCurrentTime() {
		auto now = std::chrono::steady_clock::now();
		auto nanosecondsFromEpoch = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
		return Timepoint(nanosecondsFromEpoch.count());
	}

	InputCursorMode Input::getCursorMode() {
		return InputCursorMode::NORMAL;
	}

	glm::vec2 Input::getCursorPosition() {
		return glm::vec2(0, 0);
	}

	bool Input::isKeyPressed(int32_t key) {
		return false;
	}

	bool Input::isMouseButtonPressed(int32_t mouseButton) {
		return false;
	}

}