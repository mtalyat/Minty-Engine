#include "pch.h"
#include "Font.h"

using namespace Minty;

Minty::Font::Font(FontBuilder const& builder)
	: Asset(builder.id)
	, m_name(builder.name)
	, m_variants()
{
	for (auto const& variant : builder.variants)
	{
		emplace(variant);
	}
}

void Minty::Font::emplace(Ref<FontVariant> const variant)
{
	ULong id = create_font_id(variant->get_size(), variant->is_bold(), variant->is_italic());
	m_variants.emplace(id, variant);
}

Ref<FontVariant> Minty::Font::at(UInt const size, Bool const bold, Bool const italic) const
{
	ULong id = create_font_id(size, bold, italic);
	return m_variants.at(id);
}

Ref<FontVariant> Minty::Font::get(UInt const size, Bool const bold, Bool const italic) const
{
	ULong id = create_font_id(size, bold, italic);
	auto found = m_variants.find(id);

	if (found == m_variants.end()) return nullptr;

	return m_variants.at(id);
}

std::vector<Ref<FontVariant>> Minty::Font::get_variants() const
{
	std::vector<Ref<FontVariant>> results;

	for (auto const& [_, variant] : m_variants)
	{
		results.push_back(variant);
	}

	return results;
}