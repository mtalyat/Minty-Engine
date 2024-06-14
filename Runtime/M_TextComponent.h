#pragma once
#include "Minty/Components/M_Component.h"

#include "Minty/Rendering/M_Font.h"

namespace Minty
{
	class Mesh;

	struct TextComponent
		: public Component
	{
		String text = "";

		Ref<Font> font = nullptr;

		font_size_t size = 32;

		bool bold = false;

		bool italic = false;

		Ref<Mesh> mesh = nullptr;

		void set_text(String const& newText);

		void regenerate_mesh();

	private:
		void generate_mesh();
	};
}
