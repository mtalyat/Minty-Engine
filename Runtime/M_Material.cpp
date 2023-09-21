#include "pch.h"
#include "M_Material.h"

#include "M_Renderer.h"
#include "M_Console.h"
#include <stdexcept>

using namespace minty;

minty::Material::Material(ID const shaderId, ID const textureID, Color const color)
	: shaderID(shaderId)
	, textureID(textureID)
	, color(color)
{}