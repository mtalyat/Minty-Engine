#include "pch.h"
#include "Sprite.h"

#include "Minty/Core/Math.h"
#include "Minty/Core/ToString.h"
#include "Minty/Render/Renderer.h"

using namespace Minty;

inline Minty::Sprite::Sprite(SpriteBuilder const& builder)
    : Asset(builder.id)
    , m_texture(builder.texture)
    , m_coordinateMode(builder.coordinateMode)
    , m_offset()
    , m_size()
    , m_pivot()
    , m_pixelsPerUnit()
    , m_scale()
{
    set_offset(builder.offset);
    set_size(builder.size);
    set_pivot(builder.pivot);

    set_pixels_per_unit(builder.pixelsPerUnit);
}

Float2 Minty::Sprite::get_offset() const
{
    return get_coords(m_offset);
}

void Minty::Sprite::set_offset(Float2 const offset)
{
    m_offset = set_coords(offset);
}

Float2 Minty::Sprite::get_size() const
{
    return get_coords(m_size);
}

void Minty::Sprite::set_size(Float2 const size)
{
    m_size = set_coords(size);
    
    update_scale();
}

Float2 Minty::Sprite::get_pivot() const
{
    return get_coords(m_pivot);
}

void Minty::Sprite::set_pivot(Float2 const pivot)
{
    m_pivot = set_coords(pivot);
}

void Minty::Sprite::set_pixels_per_unit(Float const ppu)
{
    MINTY_ASSERT_MESSAGE(ppu > 0.0f, "The pixels per unit of a sprite must be greater than zero.");

    m_pixelsPerUnit = ppu;
    
    update_scale();
}

Float2 Minty::Sprite::get_coords(Float2 const raw) const
{
    // translate from normalized to CoordinateMode
    switch (m_coordinateMode)
    {
    case CoordinateMode::Normalized:
        return raw;
    case CoordinateMode::Pixel:
    {
        Ref<Image> const& image = m_texture->get_image();
        UInt width = image->get_width();
        UInt height = image->get_height();
        return Float2(raw.x * width, raw.y * height);
    }
    default:
        MINTY_ABORT_FORMAT("CoordinateMode \"{}\" not implemented for get_coords().", to_string(m_coordinateMode));
    }
}

Float2 Minty::Sprite::set_coords(Float2 const raw) const
{
    // tranlate from CoordinateMode to normalized
    switch (m_coordinateMode)
    {
    case CoordinateMode::Normalized:
        return raw;
    case CoordinateMode::Pixel:
    {
        Ref<Image> image = m_texture->get_image();
        UInt width = image->get_width();
        UInt height = image->get_height();
        return Float2(raw.x / width, raw.y / height);
    }
    default:
        MINTY_ABORT_FORMAT("CoordinateMode \"{}\" not implemented for set_coords().", to_string(m_coordinateMode));
    }
}

void Minty::Sprite::update_scale()
{
    // scale so the sprite is 1 unit wide if the PPU matches the width of the sprite
    Ref<Image> image = m_texture->get_image();
    UInt width = image->get_width();

    // get size in pixels, divide that by pixels per unit to get size in units
    m_scale = (m_size.x * width) / m_pixelsPerUnit;
}

Owner<Sprite> Minty::Sprite::create(SpriteBuilder const& builder)
{
    return Owner<Sprite>(builder);
}
