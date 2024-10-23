#include "pch.h"
#include "Animation.h"

#include "Minty/Entity/EntitySerializationData.h"
#include "Minty/System/RenderSystem.h"
#include "Minty/Render/Renderer.h"
#include "Minty/Scene/Scene.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/System/SystemRegistry.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Component/Component.h"

using namespace Minty;

Minty::Animation::Animation(AnimationBuilder const& builder)
	: Asset(builder.id)
	, m_length(builder.length)
	, m_flags(
		builder.loops ? ANIMATION_FLAGS_LOOPS : ANIMATION_FLAGS_NONE
	)
	, m_entities(builder.entities)
	, m_components(builder.components)
	, m_variables(builder.variables)
	, m_values(builder.values)
	, m_times()
	, m_steps()
	, m_reset()
{
	// take steps and compile them all
	m_steps.reserve(builder.steps.size());
	for (auto const& [time, timeSteps] : builder.steps)
	{
		// get the time id
		step_time_t timeIndex = static_cast<step_time_t>(m_times.size());

		// add to times
		m_times.push_back(time);

		// add steps
		for (auto const& step : timeSteps)
		{
			step_key_t key = compile_key(step.entityIndex, step.componentIndex, step.variableIndex);

			auto found = m_steps.find(key);
			if (found == m_steps.end())
			{
				// create new map
				m_steps.emplace(key, std::unordered_map<step_time_t, step_value_t>());
			}

			step_value_t value = compile_value(step.valueIndex, step.flags);

			m_steps.at(key).emplace(timeIndex, value);
		}
	}
	m_reset.reserve(builder.resetSteps.size());
	for (auto const& step : builder.resetSteps)
	{
		// get the key and value
		step_key_t key = compile_key(step.entityIndex, step.componentIndex, step.variableIndex);
		step_value_t value = compile_value(step.valueIndex, step.flags);

		// add list if it dne for this key
		auto found = m_reset.find(key);
		if (found == m_reset.end())
		{
			m_reset.emplace(key, std::vector<step_value_t>());
		}

		// add to that list
		m_reset.at(key).push_back(value);
	}
}

Bool Minty::Animation::loops() const
{
	return m_flags & ANIMATION_FLAGS_LOOPS;
}

Bool Minty::Animation::animate(Float& time, Float const elapsedTime, Index& index, Entity const thisEntity, Scene& scene) const
{
	if (time >= m_length)
	{
		// already done
		return true;
	}

	time += elapsedTime;

	// if no more times in the times list, nothing will happen
	if (index >= m_times.size()) return false;

	// if current time < the next time in the times list, then nothing will happen
	if (time < m_times.at(index)) return false;

	// find the ending index
	Index endIndex = index;
	Index nextIndex = endIndex + 1;
	while (nextIndex < m_times.size() && time >= m_times[nextIndex])
	{
		nextIndex++;
		endIndex++;
	}

	// go through each step list
	for (auto const& [stepKey, dict] : m_steps)
	{
		// start at end, apply first value that shows up, if any
		for (int64_t i = endIndex; i >= index; i--)
		{
			auto found = dict.find(static_cast<Index>(i));
			if (found != dict.end())
			{
				// step found, so perform it and stop looking for steps since this is the last one linearly
				perform_step(stepKey, found->first, found->second, thisEntity, scene);
				break;
			}
		}
	}

	// update index, so it will start after the last step performed
	index = endIndex + 1;

	// check again if animation completed
	return time >= m_length;
}

void Minty::Animation::reset(Entity const thisEntity, Scene& scene) const
{
	// perform each step within _reset
	for (auto const& pair : m_reset)
	{
		for (auto const value : pair.second)
		{
			perform_step(pair.first, INVALID_STEP_VALUE, value, thisEntity, scene);
		}
	}
}

Size get_split(Size const index, std::vector<String> const& split, Size const defaultValue)
{
	return (index < split.size() && split.at(index).size()) ? Parse::to_size(split.at(index)) : defaultValue;
}

AnimationStep Minty::Animation::parse_step(String const& string)
{
	// split the line by /'s
	std::vector<String> parts = split(string, '/');

	// parse into the step's values
	return AnimationStep
	{
		.entityIndex = get_split(1, parts, Animation::MAX_ENTITY_INDEX),
		.componentIndex = get_split(2, parts, Animation::MAX_COMPONENT_INDEX),
		.variableIndex = get_split(3, parts, Animation::MAX_VARIABLE_INDEX),
		.timeIndex = Animation::MAX_TIME_INDEX, // time set outside of this function
		.valueIndex = get_split(4, parts, Animation::MAX_VALUE_INDEX),
		.flags = static_cast<AnimationStepFlags>(get_split(0, parts, AnimationStepFlags::ANIMATION_STEP_FLAGS_NONE)),
	};
}

Animation::step_key_t Minty::Animation::compile_key(Size const entityIndex, Size const componentIndex, Size const variableIndex) const
{
	return
		((entityIndex & MAX_ENTITY_INDEX) << ENTITY_OFFSET) |
		((componentIndex & MAX_COMPONENT_INDEX) << COMPONENT_OFFSET) |
		((variableIndex & MAX_VARIABLE_INDEX) << VARIABLE_OFFSET);
}

Animation::step_value_t Minty::Animation::compile_value(Size const valueIndex, AnimationStepFlags const flags) const
{
	return
		((valueIndex & MAX_VALUE_INDEX) << VALUE_OFFSET) |
		((flags & MAX_FLAGS_INDEX) << FLAGS_OFFSET);
}

void Minty::Animation::perform_step(step_key_t const key, step_time_t const time, step_value_t const value, Entity const thisEntity, Scene& scene) const
{
	// build the step
	AnimationStep step = AnimationStep
	{
		.entityIndex = (key >> ENTITY_OFFSET) & MAX_ENTITY_INDEX,
		.componentIndex = (key >> COMPONENT_OFFSET) & MAX_COMPONENT_INDEX,
		.variableIndex = (key >> VARIABLE_OFFSET) & MAX_VARIABLE_INDEX,
		.timeIndex = (time >> TIME_OFFSET) & MAX_TIME_INDEX,
		.valueIndex = (value >> VALUE_OFFSET) & MAX_VALUE_INDEX,
		.flags = static_cast<AnimationStepFlags>((value >> FLAGS_OFFSET) & MAX_FLAGS_INDEX),
	};

	// perform the step using that
	return perform_step(step, thisEntity, scene);
}

void Minty::Animation::perform_step(AnimationStep const& step, Entity const thisEntity, Scene& scene) const
{
	EntityRegistry& registry = scene.get_entity_registry();

	// get the entity
	// if entity index is 0xff (max ID), then it is referring to the argument Entity (this Entity, if you will)
	Entity entity = step.entityIndex == MAX_ENTITY_INDEX ? thisEntity : registry.find_by_name(m_entities.at(step.entityIndex));
	if (entity == NULL_ENTITY) return; // no entity, do not continue

	// get the component from the entity, if one was given
	if (step.componentIndex == MAX_COMPONENT_INDEX) return;
	Component* component = registry.get_by_name(m_components.at(step.componentIndex), entity);

	// determine what to do based on the flags
	if (step.flags & ANIMATION_STEP_FLAGS_ADD_REMOVE)
	{
		// remove the component from the entity if it does contain it
		if (component)
		{
			registry.erase_by_name(m_components.at(step.componentIndex), entity);
		}
	}
	else
	{
		// add the component to the entity if it does not contain it
		if (!component)
		{
			registry.emplace_by_name(m_components.at(step.componentIndex), entity);
		}
	}

	// ignore if no variable name or value
	if (step.variableIndex == MAX_VARIABLE_INDEX || step.valueIndex == MAX_VALUE_INDEX) return;

	// get the variable name that will be set
	String const& name = m_variables.at(step.variableIndex);

	// get the value to set
	Node value = m_values.at(step.valueIndex);
	value.set_name(name);

	// create a reader and deserialize using that one value
	EntitySerializationData data{
		.scene = &scene,
		.entity = thisEntity,
	};
	Node root;
	root.add_child(name, value);
	TextNodeReader reader(root);
	reader.push_data(&data);
	component->deserialize(reader);
	reader.pop_data();
}

void Minty::Animation::serialize(Writer& writer) const
{
	EntitySerializationData const* data = static_cast<EntitySerializationData const*>(writer.get_data());
	MINTY_ASSERT(data != nullptr);

	RenderSystem const* renderSystem = data->scene->get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderSystem != nullptr);

	writer.write("length", m_length);
	writer.write("loops", static_cast<Bool>(m_flags & ANIMATION_FLAGS_LOOPS));

	MINTY_TODO("Finish Animation::serialize()");
}

void Minty::Animation::deserialize(Reader& reader)
{
	EntitySerializationData const* data = static_cast<EntitySerializationData const*>(reader.get_data());
	MINTY_ASSERT(data != nullptr);

	RenderSystem const* renderer = data->scene->get_system_registry().find<RenderSystem>();
	MINTY_ASSERT(renderer != nullptr);

	MINTY_TODO("Animation::deserialize()");
}
