#pragma once

#include <NeoVoxel/Core/Base.h>
#include <NeoVoxel/Core/Event.h>
#include <NeoVoxel/Core/Time.h>

#include <vector>

namespace NeoVoxel {

	class Layer {

	public:
		Layer();
		Layer(const char* name);
		virtual ~Layer() = default;

		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;

		Layer(Layer&&) noexcept = default;
		Layer& operator=(Layer&&) noexcept = default;

		virtual void onCreate();
		virtual void onVisible();
		virtual void onCover();
		virtual void onDestroy();

		virtual void onUpdate(Timestep timestep, std::vector<EventPtr>& events);
		virtual void onRender();

		const char* getName() const noexcept { return m_Name; }

	protected:
		bool isFront() const noexcept { return m_IsFront; }

	private:
		bool m_IsFront;
		const char* m_Name;

	};

	using LayerPtr = Ptr<Layer>;

}