#pragma once

#include "Minty/Core/Constants.h"
#include "Minty/Serialize/Serializable.h"
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Manages the Layers used within the engine.
	/// </summary>
	class LayerManager
		: public Serializable
	{
	private:
		std::unordered_map<Layer, String> m_layerToNames;
		std::unordered_map<String, Layer> m_nameToLayers;

	public:
		LayerManager()
			: m_layerToNames()
			, m_nameToLayers()
		{
			m_layerToNames.reserve(LAYER_COUNT);
			m_nameToLayers.reserve(LAYER_COUNT);

			clear();
		}

		~LayerManager() = default;

	public:
		/// <summary>
		/// Sets the Layer in this LayerManager.
		/// </summary>
		/// <param name="layer">The Layer to set.</param>
		/// <param name="name">The name of the Layer.</param>
		void set(Layer const layer, String const& name);

		/// <summary>
		/// Gets the name of the given Layer.
		/// </summary>
		/// <param name="layer"></param>
		/// <returns></returns>
		String get_name(Layer const layer) const;

		/// <summary>
		/// Get the Layer of the given name.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Layer get_layer(String const& name) const;

		/// <summary>
		/// Clears the LayerManager.
		/// </summary>
		void clear();

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}