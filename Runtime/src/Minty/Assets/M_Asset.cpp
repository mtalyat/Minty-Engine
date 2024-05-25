#include "pch.h"
#include "M_Asset.h"

#include "Minty/Files/M_File.h"
#include "Minty/Tools/M_Console.h"

#include <unordered_map>
#include <unordered_set>

using namespace Minty;

Path Minty::Asset::get_meta_path(Path const& assetPath)
{
	return Path(assetPath.string() + EXTENSION_META);
}

AssetType Minty::Asset::get_type(Path const& assetPath)
{
	if (!assetPath.has_extension())
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
	};

	auto found = types.find(assetPath.extension());

	if (found == types.end()) return AssetType::None;

	return found->second;
}

std::vector<Path> const& Minty::Asset::get_extensions(AssetType const type)
{
	static std::unordered_map<AssetType, std::vector<Path>> extensions
	{
		{ AssetType::None, { } },

		{ AssetType::Meta, { EXTENSION_META } },

		{ AssetType::Text, { ".txt", ".csv" } },

		{ AssetType::Script, { ".cs" }},

		{ AssetType::Texture, { ".bmp", ".jpg", ".jpeg", ".png" }},

		{ AssetType::Sprite, { ".sprite" }},

		{ AssetType::Material, { ".material" }},

		{ AssetType::MaterialTemplate, { ".materialtemplate"}},

		{ AssetType::ShaderPass, { ".shaderpass" }},

		{ AssetType::Shader, { ".shader" }},

		{ AssetType::ShaderModule, { ".spv" }},

		{ AssetType::ShaderCode, { ".glsl", ".frag", ".vert" }},

		{ AssetType::Scene, { ".scene" }},

		{ AssetType::Mesh, { ".obj" }},

		{ AssetType::AudioClip, { ".wav", ".mp3" }},

		{ AssetType::Animation, { ".animation" }},

		{ AssetType::Animator, { ".animator" }},

		{ AssetType::Wrap, { EXTENSION_WRAP } },
	};

	auto found = extensions.find(type);

	if (found == extensions.end()) MINTY_ABORT("Unknown asset type.");

	return found->second;
}

bool Minty::Asset::is_readable(Path const& assetPath)
{
	return is_readable(get_type(assetPath));
}

bool Minty::Asset::is_readable(AssetType const type)
{
	static std::unordered_set<AssetType> readables
	{
		AssetType::Text,
		AssetType::Script,
		AssetType::Sprite,
		AssetType::Material,
		AssetType::MaterialTemplate,
		AssetType::ShaderPass,
		AssetType::Shader,
		AssetType::ShaderCode,
		AssetType::Scene,
		AssetType::Animation,
		AssetType::Animator,
	};

	return readables.contains(type);
}

bool Minty::Asset::check_type(Path const& path, AssetType const type)
{
	return get_type(path) == type;
}

bool Minty::operator==(Asset const& left, Asset const& right)
{
	return left._id == right._id;
}

bool Minty::operator!=(Asset const& left, Asset const& right)
{
	return left._id != right._id;
}
