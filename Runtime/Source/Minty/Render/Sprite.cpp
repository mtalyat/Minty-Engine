#include "pch.h"
#include "Sprite.h"

using namespace Minty;

void Minty::Sprite::set_pixels_per_unit(Float const ppu)
{
    MINTY_ASSERT_MESSAGE(ppu > 0.0f, "The pixels per unit of a sprite must be greater than zero.");

    m_pixelsPerUnit = ppu;
    
    // scale so the sprite is 1 unit wide if the PPU matches the width of the sprite
    m_scale = static_cast<Float>(m_size.x) / ppu;
}

Owner<Sprite> Minty::Sprite::create(SpriteBuilder const& builder)
{
    return Owner<Sprite>(builder);
}
