#include "pch.h"
#include "assets/M_Asset.h"

#include "files/M_File.h"
#include "tools/M_Console.h"

#include <unordered_map>
#include <unordered_set>

using namespace minty;

minty::Asset::Asset()
	: RuntimeObject()
	, _id()
	, _path()
{}

minty::Asset::Asset(UUID const id, Path const& path, Runtime& runtime)
	: RuntimeObject(runtime)
	, _id(id != INVALID_UUID ? id : UUID())
	, _path(path)
{}

minty::Asset::~Asset()
{
}

UUID minty::Asset::get_id() const
{
	return _id;
}

Path const& minty::Asset::get_path() const
{
	return _path;
}

AssetType minty::Asset::get_type() const
{
	return Asset::get_type(_path);
}

String minty::Asset::get_name() const
{
	return _path.stem().string();
}

Path minty::Asset::get_meta_path(Path const& assetPath)
{
	return Path(assetPath.string() + EXTENSION_META);
}

AssetType minty::Asset::get_type(Path const& assetPath)
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

std::vector<Path> const& minty::Asset::get_extensions(AssetType const type)
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

bool minty::Asset::is_readable(Path const& assetPath)
{
	return is_readable(get_type(assetPath));
}

bool minty::Asset::is_readable(AssetType const type)
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

bool minty::Asset::check_type(Path const& path, AssetType const type)
{
	return get_type(path) == type;
}

bool minty::operator==(Asset const& left, Asset const& right)
{
	return left._id == right._id;
}

bool minty::operator!=(Asset const& left, Asset const& right)
{
	return left._id != right._id;
}
