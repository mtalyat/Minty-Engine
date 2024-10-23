#include "pch.h"
#include "Asset.h"

#include "Minty/Core/Constants.h"

using namespace Minty;

AssetType Minty::Asset::get_type_from_path(const Path& path)
{
    if (!path.has_extension())
    {
        return AssetType::None;
    }

	static std::unordered_map<Path, AssetType> types
	{
		{ ".txt", AssetType::Text },
		{ ".csv", AssetType::Text },

		{ EXTENSION_META, AssetType::Meta },

		{ ".cs", AssetType::Script},

		{ ".bmp", AssetType::Texture },
		{ ".jpg", AssetType::Texture },
		{ ".jpeg", AssetType::Texture },
		{ ".png", AssetType::Texture },

		{ ".sprite", AssetType::Sprite },

		{ ".material", AssetType::Material },

		{ ".materialtemplate", AssetType::MaterialTemplate },

		{ ".shaderpass", AssetType::ShaderPass },

		{ ".shader", AssetType::Shader },

		{ ".spv", AssetType::ShaderModule },

		{ ".glsl", AssetType::ShaderCode },
		{ ".frag", AssetType::ShaderCode },
		{ ".vert", AssetType::ShaderCode },

		{ ".scene", AssetType::Scene },

		{ ".obj", AssetType::Mesh },

		{ ".wav", AssetType::AudioClip },
		{ ".mp3", AssetType::AudioClip },

		{ ".animation", AssetType::Animation },

		{ ".animator", AssetType::Animator },

		{ EXTENSION_WRAP, AssetType::Wrap },

		{ ".fnt", AssetType::FontVariant },
		{ ".font", AssetType::Font }
	};

	auto found = types.find(path.extension());

	// extension not found
	if (found == types.end()) return AssetType::None;

	return found->second;
}

Path Minty::Asset::get_meta_path(const Path& path)
{
	return Path(path.string() + EXTENSION_META);
}
