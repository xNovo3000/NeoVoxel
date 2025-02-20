#include <NeoVoxel/Application.h>
#include <NeoVoxel/Debug/Log.h>
#include <NeoVoxel/Debug/Profile.h>

#ifdef _WIN32
	#include <Windows.h>
#endif  // _WIN32

extern NeoVoxel::Application* createApplication();

#if NV_BUILD_MODE == NV_BUILD_MODE_DIST && defined(_WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd) {
#else  // NV_BUILD_MODE == NV_BUILD_MODE_DIST && defined(_WIN32)
int main(int argc, char** argv) {
#endif  // NV_BUILD_MODE == NV_BUILD_MODE_DIST && defined(_WIN32)
	NeoVoxel::Log::initialize();
	NV_PROFILE_START;
	auto application = createApplication();
	application->run();
	delete application;
	NV_PROFILE_STOP;
	return 0;
}