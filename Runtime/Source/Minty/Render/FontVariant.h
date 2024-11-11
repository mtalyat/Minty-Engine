#pragma once
#include "Minty/Asset/Asset.h"

#include "Minty/Render/FontChar.h"
#include "Minty/Render/Material.h"
#include "Minty/Render/Texture.h"

namespace Minty
{
	/// <summary>
	/// Used to create a new FontVariant
	/// </summary>
	struct FontVariantBuilder
	{
		UUID id = UUID::create();

		// INFO
		UInt size = 0;
		Bool bold = false;
		Bool italic = false;

		// COMMON
		Float lineHeight = 0.0f;

		// PAGES
		Ref<Texture> texture;
		Ref<Material> material;

		// CHARS
		std::vector<FontChar> characters;

		// KERNINGS
		std::unordered_map<Int, Float> kernings;
	};

	/// <summary>
	/// Holds the data for one variant of a font (size, bold, italic, etc).
	/// 
	/// FontVariants currently limited to one texture only.
	/// </summary>
	class FontVariant
		: public Asset
	{
	private:
		// INFO
		UInt m_size;
		Bool m_bold;
		Bool m_italic;

		// COMMON
		Float m_lineHeight;

		// PAGES
		Ref<Texture> m_texture;
		Ref<Material> m_material;

		// CHARS
		std::unordered_map<Char, FontChar> m_characters;

		// KERNINGS
		std::unordered_map<Int, Float> m_kernings;

	public:
		FontVariant(FontVariantBuilder const& builder);

		UInt get_size() const { return m_size; }
		Bool is_bold() const { return m_bold; }
		Bool is_italic() const { return m_italic; }
		Float get_line_height() const { return m_lineHeight; }
		Ref<Texture> get_texture() const { return m_texture; }
		Ref<Material> get_material() const { return m_material; }
		FontChar const* get_char(Char const ch) const;
		Float get_kerning(Char const left, Char const right) const;

		static Int compact_kerning(Int const left, Int const right) { return ((left & 0xffff) << 16) | (right & 0xffff); }
	};
}