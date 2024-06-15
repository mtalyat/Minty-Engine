#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Types/M_Color.h"
#include "Minty/Rendering/M_Font.h"

namespace Minty
{
	class Mesh;

	struct TextComponent
		: public Component
	{
		String text = "";

		Color color = Color::white();

		Ref<Font> font = nullptr;
		Ref<FontVariant> fontVariant = nullptr;

		font_size_t size = 32;

		bool bold = false;

		bool italic = false;

		Ref<Mesh> mesh = nullptr;

		bool dirty = true;

		void set_text(String const& newText);

		void set_size(font_size_t const newSize);

		void set_bold(bool const newBold);

		void set_italic(bool const newItalic);

		// regenerates if dirty
		bool try_regenerate_mesh();

		void regenerate_mesh();

	private:
		void generate_mesh();

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}
