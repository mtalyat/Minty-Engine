#include "pch.h"
#include "M_Asset.h"

#include "Minty/Files/M_File.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Tools/M_Text.h"

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

		{ ".fnt", AssetType::FontVariant },
		{ ".font", AssetType::Font }
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

String Minty::to_string(AssetType const value)
{
	switch (value)
	{
	case AssetType::None: return "None";
	case AssetType::Meta: return "Meta";
	case AssetType::Wrap: return "Wrap";
	case AssetType::Text: return "Text";
	case AssetType::Script: return "Script";
	case AssetType::AudioClip: return "AudioClip";
	case AssetType::Animation: return "Animation";
	case AssetType::Animator: return "Animator";
	case AssetType::Texture: return "Texture";
	case AssetType::ShaderCode: return "ShaderCode";
	case AssetType::ShaderModule: return "ShaderModule";
	case AssetType::Shader: return "Shader";
	case AssetType::ShaderPass: return "ShaderPass";
	case AssetType::MaterialTemplate: return "MaterialTemplate";
	case AssetType::Material: return "Material";
	case AssetType::Sprite: return "Sprite";
	case AssetType::Mesh: return "Mesh";
	case AssetType::Scene: return "Scene";
	default: return "";
	}
}

AssetType Minty::from_string_asset_type(String const& value)
{
	std::string value2 = Minty::Text::to_upper(value);
	if (value2 == "None") return AssetType::None;
	if (value2 == "Meta") return AssetType::Meta;
	if (value2 == "Wrap") return AssetType::Wrap;
	if (value2 == "Text") return AssetType::Text;
	if (value2 == "Script") return AssetType::Script;
	if (value2 == "AudioClip") return AssetType::AudioClip;
	if (value2 == "Animation") return AssetType::Animation;
	if (value2 == "Animator") return AssetType::Animator;
	if (value2 == "Texture") return AssetType::Texture;
	if (value2 == "ShaderCode") return AssetType::ShaderCode;
	if (value2 == "ShaderModule") return AssetType::ShaderModule;
	if (value2 == "Shader") return AssetType::Shader;
	if (value2 == "ShaderPass") return AssetType::ShaderPass;
	if (value2 == "MaterialTemplate") return AssetType::MaterialTemplate;
	if (value2 == "Material") return AssetType::Material;
	if (value2 == "Sprite") return AssetType::Sprite;
	if (value2 == "Mesh") return AssetType::Mesh;
	if (value2 == "Scene") return AssetType::Scene;

	return static_cast<AssetType>(0);
}

bool Minty::operator==(Asset const& left, Asset const& right)
{
	return left._id == right._id;
}

bool Minty::operator!=(Asset const& left, Asset const& right)
{
	return left._id != right._id;
}
