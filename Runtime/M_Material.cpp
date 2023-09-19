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
	, _buffers()
	, _memories()
	, _mapped()
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

void minty::Material::apply() const
{
	// create info to set on gpu
	MaterialInfo info =
	{
		.color = glm::vec3(_color.rf(), _color.gf(), _color.bf()),
		.textureId = _textureId
	};

	// set it to all mapped buffers
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		memcpy(_mapped[i], &info, sizeof(info));
	}
}

void minty::Material::dispose(Renderer& renderer)
{
	size_t count = _buffers.size();
	for (size_t i = 0; i < count; i++)
	{
		vkDestroyBuffer(renderer.device, _buffers[i], nullptr);
		vkFreeMemory(renderer.device, _memories[i], nullptr);
	}
}
