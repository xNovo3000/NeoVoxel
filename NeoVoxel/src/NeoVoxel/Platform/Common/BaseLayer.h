#pragma once

#include <NeoVoxel/Layer/EventListenerLayer.h>

namespace NeoVoxel {

	class BaseLayer : public EventListenerLayer {

	public:
		BaseLayer();
		virtual ~BaseLayer() = default;

		BaseLayer(const BaseLayer&) = delete;
		BaseLayer& operator=(const BaseLayer&) = delete;

		BaseLayer(BaseLayer&&) noexcept = default;
		BaseLayer& operator=(BaseLayer&&) noexcept = default;

		virtual void onRender() override;

		virtual bool onWindowCloseEvent(Timestep timestep, WindowCloseEvent& event) override;
		virtual bool onWindowSizeEvent(Timestep timestep, WindowSizeEvent& event) override;

	};

}