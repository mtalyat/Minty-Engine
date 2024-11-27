#include "pch.h"
#include "Asset.h"

#include "Minty/Core/Constants.h"
#include "Minty/Core/ApplicationInfo.h"
#include "Minty/Audio/AudioClip.h"
#include "Minty/Animation/Animation.h"
#include "Minty/Animation/Animator.h"
#include "Minty/Render/Texture.h"
#include "Minty/Render/Shader.h"
#include "Minty/Render/ShaderModule.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/MaterialTemplate.h"
#include "Minty/Render/Sprite.h"
#include "Minty/Render/Font.h"
#include "Minty/Render/FontVariant.h"
#include "Minty/Render/Mesh.h"
#include "Minty/Scene/Scene.h"

using namespace Minty;

AssetType Minty::Asset::get_type_from_path(Path const& path)
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

		{ ".mspr", AssetType::Sprite },

		{ ".mmat", AssetType::Material },

		{ ".mmtp", AssetType::MaterialTemplate },

		//{ ".shaderpass", AssetType::ShaderPass },

		{ ".msha", AssetType::Shader },

		{ ".spv", AssetType::ShaderModule },

		{ ".glsl", AssetType::ShaderCode },
		{ ".frag", AssetType::ShaderCode },
		{ ".vert", AssetType::ShaderCode },

		{ ".msce", AssetType::Scene },

		{ ".obj", AssetType::Mesh },

		{ ".wav", AssetType::AudioClip },
		{ ".mp3", AssetType::AudioClip },

		{ ".mani", AssetType::Animation },

		{ ".manr", AssetType::Animator },

		{ EXTENSION_WRAP, AssetType::Wrap },

		{ ".fnt", AssetType::FontVariant },
		{ ".mfnt", AssetType::Font }
	};

	auto found = types.find(path.extension());

	// extension not found
	if (found == types.end()) return AssetType::None;

	return found->second;
}

Path Minty::Asset::get_meta_path(Path const& path)
{
	return Path(path.string() + EXTENSION_META);
}

std::vector<Path> Minty::Asset::get_extensions(AssetType const type)
{
	static std::unordered_map<AssetType, std::vector<Path>> extensions
	{
		{ AssetType::None, { } },

		{ AssetType::Meta, { EXTENSION_META } },

		{ AssetType::Text, { ".txt", ".csv" } },

		{ AssetType::Script, { ".cs" }},

		{ AssetType::Texture, { ".bmp", ".jpg", ".jpeg", ".png" }},

		{ AssetType::Sprite, { ".mspr" }},

		{ AssetType::Material, { ".mmat" }},

		{ AssetType::MaterialTemplate, { ".mmtp"}},

		{ AssetType::Shader, { ".msha" }},

		{ AssetType::ShaderModule, { ".spv" }},

		{ AssetType::ShaderCode, { ".glsl", ".frag", ".vert" }},

		{ AssetType::Scene, { ".msce" }},

		{ AssetType::Mesh, { ".obj" }},

		{ AssetType::AudioClip, { ".wav", ".mp3" }},

		{ AssetType::Animation, { ".mani" }},

		{ AssetType::Animator, { ".manr" }},

		{ AssetType::Wrap, { EXTENSION_WRAP } },
	};

	auto found = extensions.find(type);

	if (found == extensions.end()) MINTY_ABORT("Unknown asset type.");

	return found->second;
}

AssetType Minty::asset_type_typeid(TypeID const& typeId)
{
	if (typeId == typeid(ApplicationInfo))
	{
		return AssetType::Info;
	}
	else if (typeId == typeid(AudioClip))
	{
		return AssetType::AudioClip;
	}
	else if (typeId == typeid(Animation))
	{
		return AssetType::Animation;
	}
	else if (typeId == typeid(Animator))
	{
		return AssetType::Animator;
	}
	else if (typeId == typeid(Viewport))
	{
		return AssetType::Viewport;
	}
	else if (typeId == typeid(Scissor))
	{
		return AssetType::Scissor;
	}
	else if (typeId == typeid(Texture))
	{
		return AssetType::Texture;
	}
	else if (typeId == typeid(ShaderModule))
	{
		return AssetType::ShaderModule;
	}
	else if (typeId == typeid(Shader))
	{
		return AssetType::Shader;
	}
	else if (typeId == typeid(MaterialTemplate))
	{
		return AssetType::MaterialTemplate;
	}
	else if (typeId == typeid(Material))
	{
		return AssetType::Material;
	}
	else if (typeId == typeid(Sprite))
	{
		return AssetType::Sprite;
	}
	else if (typeId == typeid(FontVariant))
	{
		return AssetType::FontVariant;
	}
	else if (typeId == typeid(Font))
	{
		return AssetType::Font;
	}
	else if (typeId == typeid(Mesh))
	{
		return AssetType::Mesh;
	}
	else if (typeId == typeid(Scene))
	{
		return AssetType::Scene;
	}
	else
	{
		return AssetType::None;
	}

}
