#pragma once
#include "Minty/Core/M_Base.h"

namespace Minty
{
	struct Time;
	class Event;

	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(Time const& time) {}
		virtual void on_event(Event& event) {}
	};

	class LayerManager
	{
	private:
		std::vector<Layer*> _layers;
		unsigned int _index;

	public:
		LayerManager() = default;
		~LayerManager();

		void push_layer(Layer* const layer);
		void push_overlay(Layer* const layer);

		void pop_layer(Layer* const layer);
		void pop_overlay(Layer* const layer);

		std::vector<Layer*>::iterator begin() { return _layers.begin(); }
		std::vector<Layer*>::iterator end() { return _layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return _layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return _layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return _layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return _layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return _layers.rend(); }
	};
}