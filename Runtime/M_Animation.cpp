#include "pch.h"
#include "M_Animation.h"

#include "M_AnimationBuilder.h"

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
	, _steps()
{
	// take steps and compile them all
	_steps.reserve(builder.steps.size());
	for (auto const& pair : builder.steps)
	{
		_steps.push_back({ pair.first, compile_step(pair.second) });
	}
}

bool minty::Animation::loops() const
{
	return _loops;
}

bool minty::Animation::animate(float& time, float const elapsedTime, size_t& index, Entity const thisEntity, EntityRegistry& registry) const
{
	if (time >= _length)
	{
		// already done
		return true;
	}

	time += elapsedTime;

	// perform all steps that are <= the time
	while (index < _steps.size() && _steps.at(index).first <= time)
	{
		// get the compiled step value
		size_t compiledStep = _steps.at(index).second;

		// get the indices for each part of the step
		Step step;
		uncompile_step(compiledStep, step);

		// get the entity
		// if entity index is 0xff (max ID), then it is referring to the argument Entity (this Entity, if you will)
		Entity entity = step.entityIndex == MAX_INDEX ? thisEntity : registry.find(_entities.at(step.entityIndex));
		if (entity == NULL_ENTITY) continue; // no entity, do not continue

		// get the component from the entity
		Component* component = registry.get_by_name(_components.at(step.componentIndex), entity);

		// determine what to do based on the type
		switch (step.type)
		{
			case StepType::Normal:
			{
				if (!component) continue; // no component, do nothing else
				break;
			}
			case StepType::Add:
			{
				// add the component to the entity if it does not contain it
				if (!component)
				{
					registry.emplace_by_name(_components.at(step.componentIndex), entity);
				}
				break;
			}
			case StepType::Remove:
			{
				// remove the component from the entity if it does contain it
				if (component)
				{
					registry.erase_by_name(_components.at(step.componentIndex), entity);
				}
				// skip the rest: cannot set component value if it was just erased
				continue;
				break;
			}
			default:
				console::error(std::format("Animation::animate(): unrecognized StepType: {}", static_cast<int>(step.type)));
				continue;
		}

		// if the value index is max value, there is no value to set
		if (step.valueIndex == MAX_INDEX) continue;

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
	// [Step type: 2 bits][Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 30 bits]
	return
		((static_cast<size_t>(step.type) & MAX_STEP) << 62) |
		((step.entityIndex & MAX_INDEX) << 54) |
		((step.componentIndex & MAX_INDEX) << 46) |
		((step.offsetIndex & MAX_INDEX) << 38) |
		((step.sizeIndex & MAX_INDEX) << 30) |
		((step.valueIndex & MAX_VALUE_INDEX));
}

void minty::Animation::uncompile_step(size_t const value, Step& step)
{
	// [Step type: 2 bits][Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 30 bits]
	step.type = static_cast<StepType>((value >> 62) & MAX_STEP);
	step.entityIndex = (value >> 54) & MAX_INDEX;
	step.componentIndex = (value >> 46) & MAX_INDEX;
	step.offsetIndex = (value >> 38) & MAX_INDEX;
	step.sizeIndex = (value >> 30) & MAX_INDEX;
	step.valueIndex = value & MAX_VALUE_INDEX;
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
