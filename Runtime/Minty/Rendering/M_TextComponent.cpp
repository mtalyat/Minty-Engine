#include "pch.h"
#include "M_TextComponent.h"

#include "Minty/Rendering/M_Mesh.h"
#include "Minty/Rendering/M_Font.h"
#include "Minty/Assets/M_AssetEngine.h"
#include "Minty/Rendering/M_Builtin.h"

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

		MINTY_ASSERT_FORMAT(fontVariant != nullptr, "There is no FontVariant for font \"{}\", size={}, bold={}, italic={}.", font->get_name(), size, bold, italic);
	}

	// create mesh based on text
	generate_mesh();
}

void Minty::TextComponent::generate_mesh()
{
	//std::vector<Vertex3D> vertices =
//{
//	// up
//	{ leftTopBack, up, topRight },
//	{ leftTopFront, up, bottomRight },
//	{ rightTopFront, up, bottomLeft },
//	{ rightTopBack, up, topLeft },
//};

//std::vector<uint16_t> indices =
//{
//	0, 1, 2, 0, 2, 3
//};

	// generate some for each character made

	//std::vector<Vertex2D> vertices;
	//std::vector<uint16_t> indices;

	//// set mesh data
	//mesh->set_vertices(vertices);
	//mesh->set_indices(indices);

	AssetEngine& assets = AssetEngine::instance();

	// create new mesh
	MINTY_ASSERT(mesh != nullptr);
	MeshBuilder builder{};
	mesh = assets.create<Mesh>(builder);

	// TEMP
	// TODO: generate mesh for text
	Mesh::create_primitive_quad(*mesh);
}
