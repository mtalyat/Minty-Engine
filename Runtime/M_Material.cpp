#include "pch.h"
#include "M_Material.h"

#include "M_Renderer.h"
#include "M_Console.h"
#include <stdexcept>

minty::Material::Material(ID const shaderId, ID const textureId)
	: _shaderId(shaderId)
	, _textureId(textureId)
{}

void minty::Material::setTexture(ID const textureId)
{
	_textureId = textureId;
}

void minty::Material::dispose(Renderer& engine)
{

}
