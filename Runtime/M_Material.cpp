#include "pch.h"
#include "M_Material.h"

#include "M_Renderer.h"
#include "M_Console.h"
#include <stdexcept>

using namespace minty;

minty::Material::Material(ID const shaderId, ID const textureId, Color const color)
	: _shaderId(shaderId)
	, _textureId(textureId)
	, _color(color)
{}

void minty::Material::setTexture(ID const textureId)
{
	_textureId = textureId;
}

void minty::Material::setColor(Color const color)
{
	_color = color;
}

Color minty::Material::getColor() const
{
	return _color;
}

void minty::Material::dispose(Renderer& renderer)
{

}
