#include "AssetUtility.h"

using namespace Minty;
using namespace Mintye;

Minty::Bool Mintye::AssetUtility::is_plain_text(Minty::AssetType const type)
{
	static std::unordered_set<AssetType> readables
	{
		AssetType::Text,
		AssetType::Script,
		AssetType::Sprite,
		AssetType::Material,
		AssetType::MaterialTemplate,
		AssetType::Shader,
		AssetType::ShaderCode,
		AssetType::Scene,
		AssetType::Animation,
		AssetType::Animator,
	};

	return readables.contains(type);
}
