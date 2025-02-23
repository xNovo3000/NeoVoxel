#include <PCH.h>
#include <NeoVoxel/Layer.h>
#include <NeoVoxel/Debug/Log.h>

namespace NeoVoxel {

	/* Layer - Singleton */

	Layer* Layer::ACTIVE_INSTANCE = nullptr;

	/* Layer */

	Layer::Layer() : Layer("Layer") {}

	Layer::Layer(const char* name) : m_Name(name), m_IsFront(false) {}

	void Layer::onCreate() {
		NV_DEBUG("{}: onCreate", m_Name);
		ACTIVE_INSTANCE = this;
	}

	void Layer::onVisible() {
		NV_DEBUG("{}: onVisible", m_Name);
		ACTIVE_INSTANCE = this;
		m_IsFront = true;
	}

	void Layer::onCover() {
		NV_DEBUG("{}: onCover", m_Name);
		ACTIVE_INSTANCE = this;
		m_IsFront = false;
	}

	void Layer::onDestroy() {
		NV_DEBUG("{}: onDestroy", m_Name);
		ACTIVE_INSTANCE = this;
	}

	void Layer::onUpdate(Timestep timestep, std::vector<EventPtr>& events) {
		NV_TRACE("{}: onUpdate", m_Name);
		ACTIVE_INSTANCE = this;
	}

	void Layer::onRender() {
		NV_TRACE("{}: onRender", m_Name);
		ACTIVE_INSTANCE = this;
	}

}