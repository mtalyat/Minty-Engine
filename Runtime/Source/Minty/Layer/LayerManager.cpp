#include "pch.h"
#include "LayerManager.h"

using namespace Minty;

static unsigned int util_layer_to_index(Layer const layer)
{
	Layer temp = 1;
	for (unsigned int i = 0; i < LAYER_COUNT; i++)
	{
		if (temp == layer)
		{
			return i;
		}
	}

	return ~0;
}

static Layer util_index_to_layer(unsigned int const index)
{
	return 1 << index;
}

void Minty::LayerManager::set(Layer const layer, String const& name)
{
	MINTY_ASSERT_MESSAGE(layer != LAYER_NONE, "Expecting a layer, instead, 0 was given.");
	MINTY_ASSERT_FORMAT(layer != LAYER_DEFAULT, "Cannot set the default layer ({}).", LAYER_DEFAULT);
	MINTY_ASSERT_FORMAT((layer & (layer - 1)) == 0, "Cannot set multiple layers at the same time ({}). Expected only one flag to be set.", layer);

	// get index
	m_layerToNames[layer] = name;
	m_nameToLayers[name] = layer;
}

String Minty::LayerManager::get_name(Layer const layer) const
{
	auto found = m_layerToNames.find(layer);

	if (found == m_layerToNames.end())
	{
		return "";
	}

	return found->second;
}

Layer Minty::LayerManager::get_layer(String const& name) const
{
	auto found = m_nameToLayers.find(name);

	if (found == m_nameToLayers.end())
	{
		return LAYER_NONE;
	}

	return found->second;
}

void Minty::LayerManager::clear()
{
	m_layerToNames.clear();
	m_nameToLayers.clear();

	m_layerToNames.emplace(LAYER_DEFAULT, "Default");
	m_nameToLayers.emplace("Default", LAYER_DEFAULT);
}

void Minty::LayerManager::serialize(Writer& writer) const
{
	// for each layer, convert to index, then write
	for (auto const& [layer, name] : m_layerToNames)
	{
		unsigned int index = util_layer_to_index(layer);
		writer.write(to_string(index), name);
	}
}

void Minty::LayerManager::deserialize(Reader& reader)
{
	// for each layer, read, then convert to bit
	String indexName;
	unsigned int index;
	String name;
	for (Size i = 0; i < reader.size(); i++)
	{
		if (reader.read_name(i, indexName))
		{
			index = Parse::to_uint(indexName);
		}
		else
		{
			MINTY_ABORT("Failed to read index of Layer.");
		}

		if (reader.read(i, name))
		{
			// get layer from index
			Layer layer = util_index_to_layer(index);

			// set value
			set(layer, name);
		}
		else
		{
			MINTY_ABORT("Failed to read name of Layer.");
		}
	}
}
