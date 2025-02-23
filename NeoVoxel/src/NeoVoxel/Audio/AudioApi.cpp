#include <PCH.h>
#include <NeoVoxel/Audio/AudioApi.h>

namespace NeoVoxel {

	AudioApi::AudioApi() : AudioApi("AudioApi") {}
	AudioApi::AudioApi(const char* name) : m_Name(name) {}

}