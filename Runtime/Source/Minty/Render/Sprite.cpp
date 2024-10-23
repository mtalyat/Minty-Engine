#include "pch.h"
#include "Sprite.h"

using namespace Minty;

Owner<Sprite> Minty::Sprite::create(SpriteBuilder const& builder)
{
    return Owner<Sprite>(builder);
}
