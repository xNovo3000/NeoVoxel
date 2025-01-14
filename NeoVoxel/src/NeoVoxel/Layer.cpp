#include <PCH.h>
#include <NeoVoxel/Layer.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	Layer::Layer() : Layer("Layer") {}

	Layer::Layer(const char* name) : m_Name(name), m_IsFront(false) {}

	void Layer::onCreate() {
		NV_DEBUG("{}: onCreate", m_Name);
	}

	void Layer::onVisible() {
		NV_DEBUG("{}: onVisible", m_Name);
		m_IsFront = true;
	}

	void Layer::onCover() {
		NV_DEBUG("{}: onCover", m_Name);
		m_IsFront = false;
	}

	void Layer::onDestroy() {
		NV_DEBUG("{}: onDestroy", m_Name);
	}

	void Layer::onUpdate(Timestep timestep, std::vector<EventPtr>& events) {
		NV_TRACE("{}: onUpdate", m_Name);
	}

	void Layer::onRender() {
		NV_TRACE("{}: onRender", m_Name);
	}

}