#pragma once

#include "Minty/Assets/M_Asset.h"
#include <vector>
#include <unordered_map>

namespace Minty
{
	typedef unsigned int font_size_t;

	/// <summary>
	/// Holds data for a character within a font.
	/// </summary>
	struct FontChar
	{
		char id;
		float x;
		float y;
		float width;
		float height;
		float xOffset;
		float yOffset;
		float xAdvance;
	};

	class Texture;
	class Material;

	/// <summary>
	/// Used to create a new FontVariant
	/// </summary>
	struct FontVariantBuilder
	{
		UUID id = UUID::create();

		Path path;

		// INFO
		font_size_t size = 0;
		bool bold = false;
		bool italic = false;

		// COMMON

		// PAGES
		Ref<Texture> texture;
		Ref<Material> material;

		// CHARS
		std::vector<FontChar> characters;

		// KERNINGS
		std::unordered_map<int, int> kernings;
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
		font_size_t _size = 0;
		bool _bold = false;
		bool _italic = false;

		// PAGES
		Ref<Texture> _texture;
		Ref<Material> _material;

		// CHARS
		std::unordered_map<char, FontChar> _characters;

		// KERNINGS
		std::unordered_map<int, int> _kernings;

	public:
		FontVariant(FontVariantBuilder const& builder);

		font_size_t get_size() const { return _size; }
		bool is_bold() const { return _bold; }
		bool is_italic() const { return _italic; }
		Ref<Texture> get_texture() const { return _texture; }
		Ref<Material> get_material() const { return _material; }
		FontChar const* get_char(char const ch) const;
		int get_kerning(char const left, char const right) const;

		static int compact_kerning(int const left, int const right) { return ((left & 0xffff) << 16) | (right & 0xffff); }
	};

	/// <summary>
	/// Used to create a new Font.
	/// </summary>
	struct FontBuilder
	{
		UUID id = UUID::create();

		Path path;

		// INFO
		String name = "";

		std::vector<Ref<FontVariant>> variants;
	};

	/// <summary>
	/// Represents a basic font that can be used to render text.
	/// </summary>
	class Font
		: public Asset
	{
	private:
		typedef unsigned long font_variant_id_t;

		String _name;
		std::unordered_map<font_variant_id_t, Ref<FontVariant>> _variants;

	public:
		Font(FontBuilder const& builder);

		void emplace(Ref<FontVariant> const variant);

		Ref<FontVariant> at(font_size_t const size, bool const bold, bool const italic) const;

		Ref<FontVariant> get(font_size_t const size, bool const bold, bool const italic) const;

		std::vector<Ref<FontVariant>> get_variants() const;

	private:
		font_variant_id_t create_font_id(font_size_t const size, bool const bold, bool const italic) const { return size | (static_cast<int>(bold) << sizeof(font_size_t)) | (static_cast<int>(bold) << (sizeof(font_size_t) + 1)); }
	};
}