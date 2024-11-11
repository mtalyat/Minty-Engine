#include "pch.h"
#include "FontVariant.h"

using namespace Minty;

Minty::FontVariant::FontVariant(FontVariantBuilder const& builder)
	: Asset(builder.id)
	, m_size(builder.size)
	, m_bold(builder.bold)
	, m_italic(builder.italic)
	, m_lineHeight(builder.lineHeight)
	, m_texture(builder.texture)
	, m_material(builder.material)
	, m_characters()
	, m_kernings(builder.kernings)
{
	// add characters using ID as key
	m_characters.reserve(builder.characters.size());

	for (auto const& ch : builder.characters)
	{
		m_characters.emplace(ch.id, ch);
	}
}

FontChar const* Minty::FontVariant::get_char(Char const ch) const
{
	auto const found = m_characters.find(ch);

	if (found == m_characters.end())
	{
		return nullptr;
	}

	return &found->second;
}

Float Minty::FontVariant::get_kerning(Char const left, Char const right) const
{
	Int kerningId = compact_kerning(left, right);
	auto const found = m_kernings.find(kerningId);

	if (found == m_kernings.end())
	{
		// default, do not adjust at all
		return 0.0f;
	}

	// adjust by some amount
	return found->second;
}