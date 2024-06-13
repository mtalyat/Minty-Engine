#include "pch.h"
#include "M_Font.h"

using namespace Minty;

Minty::FontVariant::FontVariant(FontVariantBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _size(builder.size)
	, _bold(builder.bold)
	, _italic(builder.italic)
	, _textures(builder.textures)
	, _characters(builder.characters)
	, _kernings(builder.kernings)
{ }

Minty::Font::Font(FontBuilder const& builder)
	: Asset(builder.id, builder.path)
	, _name(builder.name)
	, _variants()
{
	for (auto const variant : builder.variants)
	{
		emplace(variant);
	}
}

void Minty::Font::emplace(Ref<FontVariant> const variant)
{
	font_variant_id_t id = create_font_id(variant->get_size(), variant->is_bold(), variant->is_italic());
	_variants.emplace(id, variant);
}

Ref<FontVariant> Minty::Font::at(font_size_t const size, bool const bold, bool const italic) const
{
	font_variant_id_t id = create_font_id(size, bold, italic);
	return _variants.at(id);
}

Ref<FontVariant> Minty::Font::get(font_size_t const size, bool const bold, bool const italic) const
{
	font_variant_id_t id = create_font_id(size, bold, italic);
	auto found = _variants.find(id);

	if (found == _variants.end()) return nullptr;

	return _variants.at(id);
}

std::vector<Ref<FontVariant>> Minty::Font::get_variants() const
{
	std::vector<Ref<FontVariant>> results;

	for (auto const& [_, variant] : _variants)
	{
		results.push_back(variant);
	}

	return results;
}
