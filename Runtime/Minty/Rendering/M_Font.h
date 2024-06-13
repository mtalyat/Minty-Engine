#pragma once

#include "Minty/Core/M_Base.h"
#include "Minty/Rendering/M_Texture.h"
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
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
		int xOffset;
		int yOffset;
		int xAdvance;
		unsigned int textureIndex;
	};

	/// <summary>
	/// Used to create a new FontVariant
	/// </summary>
	struct FontVariantBuilder
	{
		// INFO
		font_size_t size = 0;
		bool bold = false;
		bool italic = false;

		// COMMON

		// PAGES
		std::vector<Ref<Texture>> textures;

		// CHARS
		std::vector<FontChar> characters;

		// KERNINGS
		std::unordered_map<int, int> kernings;

		void emplace_kerning(int const first, int const second, int const amount) { kernings.emplace((first << (sizeof(int) >> 1)) | second, amount); }
	};

	/// <summary>
	/// Holds the data for one variant of a font (size, bold, italic, etc.)
	/// </summary>
	class FontVariant
	{
	private:
		font_size_t _size = 0;
		bool _bold = false;
		bool _italic = false;

		// PAGES
		std::vector<Ref<Texture>> _textures;

		// CHARS
		std::vector<FontChar> _characters;

		// KERNINGS
		std::unordered_map<int, int> _kernings;

	public:
		FontVariant(FontVariantBuilder const& builder);

		font_size_t get_size() const { return _size; }
		bool is_bold() const { return _bold; }
		bool is_italic() const { return _italic; }
	};

	/// <summary>
	/// Used to create a new Font.
	/// </summary>
	struct FontBuilder
	{
		// INFO
		String name = "";

		std::vector<Ref<FontVariant>> variants;
	};

	/// <summary>
	/// Represents a basic font that can be used to render text.
	/// </summary>
	class Font
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

	private:
		font_variant_id_t create_font_id(font_size_t const size, bool const bold, bool const italic) const { return size | (static_cast<int>(bold) << sizeof(font_size_t)) | (static_cast<int>(bold) << (sizeof(font_size_t) + 1)); }
	};
}