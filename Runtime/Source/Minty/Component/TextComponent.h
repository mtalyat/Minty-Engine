#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Render/Font.h"

namespace Minty
{
	/// <summary>
	/// Holds Text data for an Entity.
	/// </summary>
	struct TextComponent
		: public Component
	{
		/// <summary>
		/// The text to render.
		/// </summary>
		String text = "";

		/// <summary>
		/// The color of the text.
		/// </summary>
		Color color = Color::white();

		/// <summary>
		/// The font to render the text in.
		/// </summary>
		Ref<Font> font = nullptr;

		/// <summary>
		/// The specific variant of the font to render the text in.
		/// </summary>
		Ref<FontVariant> fontVariant = nullptr;

		/// <summary>
		/// The size of the font to render.
		/// </summary>
		UInt fontSize = 32;

		/// <summary>
		/// If true, bold the text.
		/// </summary>
		Bool bold = false;

		/// <summary>
		/// If true, italicize the text.
		/// </summary>
		Bool italic = false;

		/// <summary>
		/// A reference to the generated text mesh.
		/// </summary>
		Ref<Mesh> mesh = nullptr;

		/// <summary>
		/// If true, the mesh needs to be regenerated.
		/// </summary>
		Bool dirty = true;

		/// <summary>
		/// Sets the text and dirties the component.
		/// </summary>
		/// <param name="newText">The new value to set.</param>
		void set_text(String const& newText);

		/// <summary>
		/// Sets the size and dirties the component.
		/// </summary>
		/// <param name="newSize">The new value to set.</param>
		void set_size(UInt const newSize);

		/// <summary>
		/// Sets the bold and dirties the component.
		/// </summary>
		/// <param name="newBold">The new value to set.</param>
		void set_bold(Bool const newBold);

		/// <summary>
		/// Sets the italic and dirties the component.
		/// </summary>
		/// <param name="newItalic">The new value to set.</param>
		void set_italic(Bool const newItalic);

		/// <summary>
		/// Regenerates the mesh if this component is dirty.
		/// </summary>
		/// <returns>True on regeneration.</returns>
		Bool try_regenerate_mesh();

		/// <summary>
		/// Regenerates the mesh.
		/// </summary>
		void regenerate_mesh();

	private:
		/// <summary>
		/// Generates the Mesh. Assumes the Mesh is null.
		/// </summary>
		void generate_mesh();

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}