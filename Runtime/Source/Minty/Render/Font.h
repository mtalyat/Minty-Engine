#pragma once
#include "Minty/Asset/Asset.h"

#include "Minty/Render/FontVariant.h"

namespace Minty
{
	/// <summary>
	/// Used to create a new Font.
	/// </summary>
	struct FontBuilder
	{
		UUID id = UUID::create();

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
		String m_name;
		std::unordered_map<ULong, Ref<FontVariant>> m_variants;

	public:
		Font(FontBuilder const& builder);

		void emplace(Ref<FontVariant> const variant);

		String const& get_name() const { return m_name; }

		Ref<FontVariant> at(UInt const size, Bool const bold, Bool const italic) const;

		Ref<FontVariant> get(UInt const size, Bool const bold, Bool const italic) const;

		std::vector<Ref<FontVariant>> get_variants() const;

	private:
		ULong create_font_id(UInt const size, Bool const bold, Bool const italic) const { return size | (static_cast<Int>(bold) << sizeof(UInt)) | (static_cast<Int>(bold) << (sizeof(UInt) + 1)); }

	public:
		AssetType get_type() const override { return AssetType::Font; }

	public:
		static Owner<Font> create(FontBuilder const& builder);
	};
}