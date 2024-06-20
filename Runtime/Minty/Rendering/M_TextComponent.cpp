#include "pch.h"
#include "M_TextComponent.h"

#include "Minty/Rendering/M_Mesh.h"
#include "Minty/Rendering/M_Font.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Rendering/M_Builtin.h"
#include "Minty/Rendering/M_Texture.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Serialization/M_Reader.h"

using namespace Minty;
using namespace Minty::Builtin;

void Minty::TextComponent::set_text(String const& newText)
{
	// replace only if needed
	if (newText != text)
	{
		text = newText;
		dirty = true;
	}
}

void Minty::TextComponent::set_size(font_size_t const newSize)
{
	if (newSize != size)
	{
		size = newSize;
		dirty = true;
	}
}

void Minty::TextComponent::set_bold(bool const newBold)
{
	if (newBold != bold)
	{
		bold = newBold;
		dirty = true;
	}
}

void Minty::TextComponent::set_italic(bool const newItalic)
{
	if (newItalic != italic)
	{
		italic = newItalic;
		dirty = true;
	}
}

bool Minty::TextComponent::try_regenerate_mesh()
{
	if (dirty)
	{
		regenerate_mesh();
		dirty = false;
		return true;
	}

	return false;
}

void Minty::TextComponent::regenerate_mesh()
{
	AssetEngine& assets = AssetEngine::instance();

	// destroy (unload) old mesh
	if (mesh != nullptr)
	{
		assets.unload(mesh->get_id());
		mesh = nullptr;
	}

	// if text is empty, do nothing
	if (text.empty()) return;

	// ensure there is a font variant for this size, and update it
	if (font.get())
	{
		fontVariant = font->at(size, bold, italic);
	}

	// create mesh based on text
	generate_mesh();
}

void Minty::TextComponent::generate_mesh()
{
	AssetEngine& assets = AssetEngine::instance();

	// create new mesh
	MINTY_ASSERT(mesh == nullptr);
	MeshBuilder builder
	{
		.path = "TextMesh"
	};
	mesh = assets.create<Mesh>(builder);

	if (text.empty()) return;

	MINTY_ASSERT_FORMAT(font != nullptr, "There is no Font for text \"{}\".", text);
	MINTY_ASSERT_FORMAT(fontVariant != nullptr, "There is no FontVariant for font \"{}\" for text \"{}\", size={}, bold={}, italic={}.", font->get_name(), text, size, bold, italic);

	std::vector<Vertex2D> vertices;
	std::vector<uint16_t> indices;

	float xAdvance = 0.0f;
	float yAdvance = 0.0f;
	uint16_t index = 0;
	float const width = static_cast<float>(fontVariant->get_texture()->get_width());
	float const height = static_cast<float>(fontVariant->get_texture()->get_height());

	char last = '\0';

	for (char c : text)
	{
		// special characters
		bool cont = true;
		switch (c)
		{
		case '\n':
			yAdvance += fontVariant->get_line_height();
			xAdvance = 0;
			break;
		default:
			cont = false;
			break;
		}

		// if special character handled, skip, keep going
		if (cont)
		{
			last = c;
			continue;
		}

		// get font character data
		FontChar const* fc = fontVariant->get_char(c);

		if (!fc)
		{
			MINTY_ERROR_FORMAT("There is no FontChar data for character \"{}\" in font \"{}\".", c, font->get_name());

			last = c;

			continue;
		}

		Vector2 const min(fc->x, fc->y);
		Vector2 const max(fc->x + fc->width, fc->y + fc->height);
		Vector2 const offset(fc->xOffset, fc->yOffset);
		
		// adjust spacing for special cases
		xAdvance += fontVariant->get_kerning(last, c);

		// create vertices based on each char
		vertices.push_back(Vertex2D{
			.pos = Vector2(xAdvance, yAdvance) + offset,
			.coord = min
			});
		vertices.push_back(Vertex2D{
			.pos = Vector2(xAdvance + fc->width, yAdvance) + offset,
			.coord = Vector2(max.x, min.y)
			});
		vertices.push_back(Vertex2D{
			.pos = Vector2(xAdvance + fc->width, yAdvance +fc->height) + offset,
			.coord = max
			});
		vertices.push_back(Vertex2D{
			.pos = Vector2(xAdvance, yAdvance + fc->height) + offset,
			.coord = Vector2(min.x, max.y)
			});

		// create indices, always in the same order
		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		index += 4;

		// advance the "cursor"
		xAdvance += fc->xAdvance;

		// update new last char
		last = c;
	}

	// set mesh data
	mesh->set_vertices(vertices);
	mesh->set_indices(indices);
}

void Minty::TextComponent::serialize(Writer& writer) const
{
	writer.write("text", text);
	writer.write("color", color);
	writer.write("size", size);
	writer.write("bold", bold);
	writer.write("italic", italic);
	writer.write("font", font == nullptr ? UUID() : font->get_id());
}

void Minty::TextComponent::deserialize(Reader const& reader)
{
	if (reader.try_read_string("text", text)) dirty = true;
	reader.try_read_color("color", color);
	if (reader.try_read_uint("size", size)) dirty = true;
	if (reader.try_read_bool("bold", bold)) dirty = true;
	if (reader.try_read_bool("italic", italic)) dirty = true;

	UUID fontId;
	if (reader.try_read_uuid("font", fontId))
	{
		AssetEngine& assets = AssetEngine::instance();
		font = assets.get<Font>(fontId);
		fontVariant = nullptr;
		dirty = true;
	}
}
