#include "pch.h"
#include "M_Animation.h"

#include "M_SerializationData.h"
#include "M_RenderSystem.h"
#include "M_RenderEngine.h"

using namespace minty;

minty::Animation::Animation()
	: _length()
	, _entities()
	, _components()
	, _sizes()
	, _values()
	, _steps()
{}

minty::Animation::Animation(AnimationBuilder const& builder)
	: _length(builder.length)
	, _entities(builder.entities)
	, _components(builder.components)
	, _sizes(builder.sizes)
	, _values(builder.values)
	, _steps(builder.steps)
{}

bool minty::Animation::animate(float& time, float const elapsedTime, size_t& index, Entity const thisEntity, EntityRegistry& registry) const
{
	if (time >= _length)
	{
		// already done
		return true;
	}

	time += elapsedTime;

	// perform all steps that are <= the time
	while (_steps.at(index).first <= time)
	{
		// get the compiled step value
		size_t compiledStep = _steps.at(index).second;

		// get the indices for each part of the step
		Step step;
		uncompile_step(compiledStep, step);

		// get the entity
		// if entity index is 0xff (max ID), then it is referring to the argument Entity (this Entity, if you will)
		Entity entity = step.entityIndex == 0xff ? thisEntity : registry.find(_entities.at(step.entityIndex));
		if (entity == NULL_ENTITY) continue; // no entity, do not continue

		// get the component from the entity
		Component* component = registry.get_by_name(_components.at(step.componentIndex), entity);
		if (!component) continue; // no component, do not continue

		// get the value to set
		Dynamic const& value = _values.at(step.valueIndex);

		// set the value using the offset and size
		size_t offset = _sizes.at(step.offsetIndex);
		size_t size = _sizes.at(step.sizeIndex);

		memcpy(static_cast<char*>(static_cast<void*>(component)) + offset, value.data(), size);

		// move to the next step
		index++;
	}

	if (time >= _length)
	{
		// now its done
		return true;
	}

	// not done yet
	return false;
}

uint64_t minty::Animation::compile_step(Step const& step)
{
	// [Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 32 bits]
	return
		((step.entityIndex & 0xff) << 56) |
		((step.componentIndex & 0xff) << 48) |
		((step.offsetIndex & 0xff) << 40) |
		((step.sizeIndex & 0xff) << 32) |
		((step.valueIndex & 0xffffffff));
}

void minty::Animation::uncompile_step(size_t const value, Step& step)
{
	// [Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 32 bits]
	step.entityIndex = (value >> 56) & 0xff;
	step.componentIndex = (value >> 48) & 0xff;
	step.offsetIndex = (value >> 40) & 0xff;
	step.sizeIndex = (value >> 32) & 0xff;
	step.valueIndex = value & 0xffffffff;
}

void minty::Animation::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderSystem const* renderer = data->scene->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(renderer != nullptr, "Animation::serialize(): RenderSystem cannot be null.");

	
}

void minty::Animation::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	RenderSystem const* renderer = data->scene->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(renderer != nullptr, "Animation::deserialize(): RenderSystem cannot be null.");

	
}
