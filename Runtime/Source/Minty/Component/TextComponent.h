#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Render/Font.h"

namespace Minty
{
	struct TextComponent
		: public Component
	{
		String text = "";

		Color color = Color::white();

		Ref<Font> font = nullptr;
		Ref<FontVariant> fontVariant = nullptr;
		UInt fontSize = 32;

		Bool bold = false;
		Bool italic = false;

		Ref<Mesh> mesh = nullptr;
		Bool dirty = true;

		void set_text(String const& newText);
		void set_size(UInt const newSize);
		void set_bold(Bool const newBold);
		void set_italic(Bool const newItalic);

		// regenerates if dirty
		bool try_regenerate_mesh();

		void regenerate_mesh();

	private:
		void generate_mesh();

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}