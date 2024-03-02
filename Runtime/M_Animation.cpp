#include "pch.h"
#include "M_Animation.h"

#include "M_SerializationData.h"
#include "M_RenderSystem.h"
#include "M_RenderEngine.h"
#include "M_Scene.h"
#include "M_EntityRegistry.h"
#include "M_SystemRegistry.h"
#include "M_Dynamic.h"
#include "M_Component.h"
#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::Animation::Animation()
	: Asset()
	, _length()
	, _flags()
	, _entities()
	, _components()
	, _variables()
	, _values()
	, _times()
	, _steps()
	, _reset()
{}

minty::Animation::Animation(AnimationBuilder const& builder, Runtime& runtime)
	: Asset(builder.id, builder.path, runtime)
	, _length(builder.length)
	, _flags(
		builder.loops ? ANIMATION_FLAGS_LOOPS : ANIMATION_FLAGS_NONE
	)
	, _entities(builder.entities)
	, _components(builder.components)
	, _variables(builder.variables)
	, _values(builder.values)
	, _times()
	, _steps()
	, _reset()
{
	// take steps and compile them all
	_steps.reserve(builder.steps.size());
	for (auto const& pair : builder.steps)
	{
		// if this is the first time, or a new time, add to times
		if (!_times.size() || _times.back() != pair.first)
		{
			_times.push_back(pair.first);
		}

		// get that time id
		step_time_t timeIndex = static_cast<step_time_t>(_times.size() - 1);

		// add to steps
		step_key_t key = compile_key(pair.second.entityIndex, pair.second.componentIndex, pair.second.variableIndex);
		
		auto found = _steps.find(key);
		if (found == _steps.end())
		{
			// create new map
			_steps.emplace(key, std::unordered_map<step_time_t, step_value_t>());
		}

		step_value_t value = compile_value(pair.second.valueIndex, pair.second.flags);

		// add to map
		_steps.at(key).emplace(timeIndex, value);
	}
	_reset.reserve(builder.resetSteps.size());
	for (auto const& step : builder.resetSteps)
	{
		// get the key and value
		step_key_t key = compile_key(step.entityIndex, step.componentIndex, step.variableIndex);
		step_value_t value = compile_value(step.valueIndex, step.flags);

		// add list if it dne for this key
		auto found = _reset.find(key);
		if (found == _reset.end())
		{
			_reset.emplace(key, std::vector<step_value_t>());
		}

		// add to that list
		_reset.at(key).push_back(value);
	}
}

bool minty::Animation::loops() const
{
	return _flags & ANIMATION_FLAGS_LOOPS;
}

bool minty::Animation::animate(float& time, float const elapsedTime, Index& index, Entity const thisEntity, Scene& scene) const
{
	if (time >= _length)
	{
		// already done
		return true;
	}

	time += elapsedTime;

	// if no more times in the times list, nothing will happen
	if (index >= _times.size()) return false;

	// if current time < the next time in the times list, then nothing will happen
	if (time < _times.at(index)) return false;
	
	// find the ending index
	Index endIndex = index;
	Index nextIndex = endIndex + 1;
	while (nextIndex < _times.size() && time >= _times[nextIndex])
	{
		nextIndex++;
		endIndex++;
	}

	// go through each step list
	for (auto const& pair : _steps)
	{
		// start at end, apply first value that shows up, if any
		for (int64_t i = endIndex; i >= index; i--)
		{
			auto found = pair.second.find(static_cast<Index>(i));
			if (found != pair.second.end())
			{
				// step found, so perform it and stop looking for steps since this is the last one linearly
				perform_step(pair.first, found->first, found->second, thisEntity, scene);
				break;
			}
		}
	}

	// update index, so it will start after the last step performed
	index = endIndex + 1;

	// check again if animation completed
	return time >= _length;
}

void minty::Animation::reset(Entity const thisEntity, Scene& scene) const
{
	// perform each step within _reset
	for (auto const& pair : _reset)
	{
		for (auto const value : pair.second)
		{
			perform_step(pair.first, INVALID_STEP_VALUE, value, thisEntity, scene);
		}
	}
}

size_t get_split(size_t const index, std::vector<String> const& split, size_t const defaultValue)
{
	return (index < split.size() && split.at(index).size()) ? Parse::to_size(split.at(index)) : defaultValue;
}

AnimationStep minty::Animation::parse_step(String const& string)
{
	// split the line by /'s
	std::vector<String> split = Text::split(string, '/');

	// parse into the step's values
	return AnimationStep
	{
		.entityIndex = get_split(1, split, Animation::MAX_ENTITY_INDEX),
		.componentIndex = get_split(2, split, Animation::MAX_COMPONENT_INDEX),
		.variableIndex = get_split(3, split, Animation::MAX_VARIABLE_INDEX),
		.timeIndex = Animation::MAX_TIME_INDEX, // time set outside of this function
		.valueIndex = get_split(4, split, Animation::MAX_VALUE_INDEX),
		.flags = static_cast<AnimationStepFlags>(get_split(0, split, AnimationStepFlags::ANIMATION_STEP_FLAGS_NONE)),
	};
}

Animation::step_key_t minty::Animation::compile_key(size_t const entityIndex, size_t const componentIndex, size_t const variableIndex) const
{
	return
		((entityIndex & MAX_ENTITY_INDEX) << ENTITY_OFFSET) |
		((componentIndex & MAX_COMPONENT_INDEX) << COMPONENT_OFFSET) |
		((variableIndex & MAX_VARIABLE_INDEX) << VARIABLE_OFFSET);
}

Animation::step_value_t minty::Animation::compile_value(size_t const valueIndex, AnimationStepFlags const flags) const
{
	return
		((valueIndex & MAX_VALUE_INDEX) << VALUE_OFFSET) |
		((flags & MAX_FLAGS_INDEX) << FLAGS_OFFSET);
}

void minty::Animation::perform_step(step_key_t const key, step_time_t const time, step_value_t const value, Entity const thisEntity, Scene& scene) const
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

void minty::Animation::perform_step(AnimationStep const& step, Entity const thisEntity, Scene& scene) const
{
	EntityRegistry& registry = scene.get_entity_registry();

	// get the entity
	// if entity index is 0xff (max ID), then it is referring to the argument Entity (this Entity, if you will)
	Entity entity = step.entityIndex == MAX_ENTITY_INDEX ? thisEntity : registry.find_by_name(_entities.at(step.entityIndex));
	if (entity == NULL_ENTITY) return; // no entity, do not continue

	// get the component from the entity, if one was given
	if(step.componentIndex == MAX_COMPONENT_INDEX) return;
	Component* component = registry.get_by_name(_components.at(step.componentIndex), entity);

	// determine what to do based on the flags
	if (step.flags & ANIMATION_STEP_FLAGS_ADD_REMOVE)
	{
		// remove the component from the entity if it does contain it
		if (component)
		{
			registry.erase_by_name(_components.at(step.componentIndex), entity);
		}
	}
	else
	{
		// add the component to the entity if it does not contain it
		if (!component)
		{
			registry.emplace_by_name(_components.at(step.componentIndex), entity);
		}
	}

	// ignore if no variable name or value
	if (step.variableIndex == MAX_VARIABLE_INDEX || step.valueIndex == MAX_VALUE_INDEX) return;

	// get the variable name that will be set
	String const& name = _variables.at(step.variableIndex);

	// get the value to set
	Node value = _values.at(step.valueIndex);
	value.set_name(name);

	// create a reader and deserialize using that one value
	SerializationData data = SerializationData{
		.scene = &scene,
		.entity = thisEntity,
	};
	Node root;
	root.add_child(value);
	Reader reader(root, &data);
	component->deserialize(reader);
}

void minty::Animation::serialize(Writer& writer) const
{
	SerializationData const* data = static_cast<SerializationData const*>(writer.get_data());
	RenderSystem const* renderer = data->scene->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(data != nullptr);
	MINTY_ASSERT(renderer != nullptr);

	writer.write("length", _length);
	writer.write("loops", static_cast<bool>(_flags & ANIMATION_FLAGS_LOOPS));
	
	Console::todo("Finish Animation::serialize()");
}

void minty::Animation::deserialize(Reader const& reader)
{
	SerializationData const* data = static_cast<SerializationData const*>(reader.get_data());
	RenderSystem const* renderer = data->scene->get_system_registry().find<RenderSystem>();

	MINTY_ASSERT(data != nullptr);
	MINTY_ASSERT(renderer != nullptr);

	Console::todo("Animation::deserialize()");
}
