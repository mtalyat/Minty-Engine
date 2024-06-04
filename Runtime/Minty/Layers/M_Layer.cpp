#include "pch.h"
#include "M_Layer.h"

using namespace Minty;

Minty::LayerManager::~LayerManager()
{
	for (Layer* layer : _layers)
	{
		layer->on_detach();
		delete layer;
	}
}

void Minty::LayerManager::push_layer(Layer* const layer)
{
	MINTY_ASSERT(layer);

	_layers.emplace(_layers.begin() + _index, layer);
	_index++;
	layer->on_attach();
}

void Minty::LayerManager::push_overlay(Layer* const layer)
{
	MINTY_ASSERT(layer);

	_layers.emplace_back(layer);
	layer->on_attach();
}

void Minty::LayerManager::pop_layer(Layer* const layer)
{
	MINTY_ASSERT(layer);

	auto it = std::find(_layers.begin(), _layers.begin() + _index, layer);
	if (it != _layers.begin() + _index)
	{
		layer->on_detach();
		_layers.erase(it);
		_index--;
	}
}

void Minty::LayerManager::pop_overlay(Layer* const layer)
{
	MINTY_ASSERT(layer);

	auto it = std::find(_layers.begin() + _index, _layers.end(), layer);
	if (it != _layers.end())
	{
		layer->on_detach();
		_layers.erase(it);
	}
}

