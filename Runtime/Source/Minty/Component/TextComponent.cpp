#include "pch.h"
#include "TextComponent.h"

#include "Minty/Asset/AssetManager.h"

using namespace Minty;

void Minty::TextComponent::set_text(String const& newText)
{
	// replace only if needed
	if (newText != text)
	{
		text = newText;
		dirty = true;
	}
}

void Minty::TextComponent::set_size(UInt const newSize)
{
	if (newSize != fontSize)
	{
		fontSize = newSize;
		dirty = true;
	}
}

void Minty::TextComponent::set_bold(Bool const newBold)
{
	if (newBold != bold)
	{
		bold = newBold;
		dirty = true;
	}
}

void Minty::TextComponent::set_italic(Bool const newItalic)
{
	if (newItalic != italic)
	{
		italic = newItalic;
		dirty = true;
	}
}

Bool Minty::TextComponent::try_regenerate_mesh()
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
	// destroy (unload) old mesh
	if (mesh != nullptr)
	{
		AssetManager::unload(mesh->id());
		mesh = nullptr;
	}

	// if text is empty, do nothing
	if (text.empty()) return;

	// ensure there is a font variant for this fontSize, and update it
	if (font.get())
	{
		fontVariant = font->at(fontSize, bold, italic);
	}

	// create mesh based on text
	generate_mesh();
}

void Minty::TextComponent::generate_mesh()
{
	MINTY_ASSERT(mesh == nullptr);

	// if no text, no mesh
	if (text.empty()) return;

	// if no font, no mesh
	if (font == nullptr || fontVariant == nullptr) return;

	DynamicContainer vertices;
	DynamicContainer indices;

	Float xAdvance = 0.0f;
	Float yAdvance = 0.0f;
	UShort index = 0;
	
	Ref<Texture> fontVariantTexture = fontVariant->get_texture();
	Float const width = static_cast<Float>(fontVariantTexture->get_width());
	Float const height = static_cast<Float>(fontVariantTexture->get_height());

	Char last = '\0';

	for (Char c : text)
	{
		// special characters
		Bool cont = true;
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

		Float2 const min(fc->x, fc->y);
		Float2 const max(fc->x + fc->width, fc->y + fc->height);
		Float2 const offset(fc->xOffset, fc->yOffset);

		// adjust spacing for special cases
		xAdvance += fontVariant->get_kerning(last, c);

		// create vertices based on each Char
		vertices.append_object(Float2(xAdvance, yAdvance) + offset);
		vertices.append_object(min);

		vertices.append_object(Float2(xAdvance + fc->width, yAdvance) + offset);
		vertices.append_object(Float2(max.x, min.y));

		vertices.append_object(Float2(xAdvance + fc->width, yAdvance + fc->height) + offset);
		vertices.append_object(max);

		vertices.append_object(Float2(xAdvance, yAdvance + fc->height) + offset);
		vertices.append_object(Float2(min.x, max.y));

		// create indices, always in the same order
		indices.append_object(index);
		indices.append_object(index + 1);
		indices.append_object(index + 2);
		indices.append_object(index);
		indices.append_object(index + 2);
		indices.append_object(index + 3);

		index += 4;

		// advance the "cursor"
		xAdvance += fc->xAdvance;

		// update new last Char
		last = c;
	}

	// create the new mesh
	MeshBuilder builder{};
	builder.id = UUID::create();
	builder.vertexCount = index;
	builder.vertexData = vertices.data();
	builder.vertexStride = sizeof(Float2) * 2;
	builder.indexCount = (index * 3) >> 2; // 6 indices for every 4 vertices
	builder.indexData = indices.data();
	builder.indexStride = sizeof(UShort);
	mesh = AssetManager::create<Mesh>(builder);
}

void Minty::TextComponent::serialize(Writer& writer) const
{
	writer.write("text", text);
	writer.write("color", color);
	writer.write("fontSize", fontSize);
	writer.write("bold", bold);
	writer.write("italic", italic);
	writer.write("font", font == nullptr ? UUID() : font->id());
}

void Minty::TextComponent::deserialize(Reader& reader)
{
	if (reader.read("text", text)) dirty = true;
	reader.read("color", color);
	if (reader.read("fontSize", fontSize)) dirty = true;
	if (reader.read("bold", bold)) dirty = true;
	if (reader.read("italic", italic)) dirty = true;

	UUID fontId;
	if (reader.read("font", fontId))
	{
		font = AssetManager::get<Font>(fontId);
		fontVariant = nullptr;
		dirty = true;
	}
}
