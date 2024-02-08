#pragma once

#include "M_Object.h"
#include "M_Entity.h"
#include <vector>
#include <unordered_map>

namespace minty
{
	struct AnimationBuilder;
	class Scene;

	/// <summary>
	/// Holds data for an animation that can be ran.
	/// </summary>
	class Animation
		: public Object
	{
	private:
		enum Flags : int
		{
			ANIMATION_FLAGS_NONE = 0,
			ANIMATION_FLAGS_LOOPS = 0b00000001,
			ANIMATION_FLAGS_ALL = 0b00000001,
		};

	private:
		constexpr static int ENTITY_OFFSET = 16;
		constexpr static int COMPONENT_OFFSET = 8;
		constexpr static int VARIABLE_OFFSET = 0;
		constexpr static int TIME_OFFSET = 0;
		constexpr static int VALUE_OFFSET = 4;
		constexpr static int FLAGS_OFFSET = 0;
		constexpr static size_t MAX_ENTITY_INDEX = 0xff;
		constexpr static size_t MAX_COMPONENT_INDEX = 0xff;
		constexpr static size_t MAX_VARIABLE_INDEX = 0xff;
		constexpr static size_t MAX_TIME_INDEX = 0xffffffff;
		constexpr static size_t MAX_VALUE_INDEX = 0xfffffff;
		constexpr static size_t MAX_FLAGS_INDEX = 0xf;
		typedef uint32_t step_key_t;
		typedef uint32_t step_time_t;
		typedef uint32_t step_value_t;
		constexpr static step_key_t INVALID_STEP_KEY = (MAX_ENTITY_INDEX << ENTITY_OFFSET) | (MAX_COMPONENT_INDEX << COMPONENT_OFFSET) | (MAX_VARIABLE_INDEX | VARIABLE_OFFSET);
		constexpr static step_time_t INVALID_STEP_TIME = (MAX_TIME_INDEX << TIME_OFFSET);
		constexpr static step_value_t INVALID_STEP_VALUE = (MAX_VALUE_INDEX << VALUE_OFFSET) | (MAX_FLAGS_INDEX << FLAGS_OFFSET);

	public:
		enum StepFlags
		{
			ANIMATION_STEP_FLAGS_NONE = 0b0000,
			ANIMATION_STEP_FLAGS_ADD_REMOVE = 0b0001,
			ANIMATION_STEP_FLAGS_ALL = 0b0001,
		};

		/// <summary>
		/// Holds all of the indices required for a step, to different component values.
		/// </summary>
		struct Step
		{
			size_t entityIndex = MAX_ENTITY_INDEX;
			size_t componentIndex = MAX_COMPONENT_INDEX;
			size_t variableIndex = MAX_VARIABLE_INDEX;
			size_t timeIndex = MAX_TIME_INDEX;
			size_t valueIndex = MAX_VALUE_INDEX;
			StepFlags flags;
		};

		typedef uint32_t Index;
	private:
		/// <summary>
		/// The amount of time this Animation runs for, in seconds.
		/// </summary>
		float _length;

		Flags _flags;

		/// <summary>
		/// A list of all Entities being affected by this Animation.
		/// </summary>
		std::vector<String> _entities;

		/// <summary>
		/// A list of all Components being affected by this Animation.
		/// </summary>
		std::vector<String> _components;

		/// <summary>
		/// A list of all variable names being used by this Animation.
		/// </summary>
		std::vector<String> _variables;

		/// <summary>
		/// List of all time intervals in this Animation.
		/// </summary>
		std::vector<float> _times;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Node> _values;

		/// <summary>
		/// The compilation of steps within this Animation.
		/// [Entity index: 8 bits][Component index: 8 bits][Offset index: 8 bits][Time index: 18 bits][Value index: 18 bits][Flags: 4 bits]
		/// 
		/// Key: The combined IDs of Entity, Component, Variable
		/// Value Key: Time ID
		/// Value Value: Value and Flags
		/// </summary>
		std::unordered_map<step_key_t, std::unordered_map<step_time_t, step_value_t>> _steps;

		/// <summary>
		/// The steps taken when the animation is reset.
		/// </summary>
		std::unordered_map<step_key_t, std::vector<step_value_t>> _reset;

	public:
		/// <summary>
		/// Creates an empty animation.
		/// </summary>
		Animation();

		/// <summary>
		/// Creates an animation with the given frames.
		/// </summary>
		/// <param name="frames">A list of Sprite IDs.</param>
		/// <param name="frameTime">The time to elapse per frame.</param>
		Animation(AnimationBuilder const& builder);

		/// <summary>
		/// Checks if this Animation loops.
		/// </summary>
		/// <returns></returns>
		bool loops() const;

		/// <summary>
		/// Performs an animation on an Entity.
		/// </summary>
		/// <param name="time">The current time. This will be updated.</param>
		/// <param name="elapsedTime">The time that has elapsed over the last frame.</param>
		/// <param name="index">The index of the next step to perform.</param>
		/// <param name="thisEntity">The entity in which this Animation is being acted upon.</param>
		/// <param name="registry">The EntityRegistry in which thisEntity is in.</param>
		/// <returns>True when the animation has completed, otherwise false.</returns>
		bool animate(float& time, float const elapsedTime, Index& index, Entity const thisEntity, Scene& scene) const;

		/// <summary>
		/// Performs a reset on an Entity.
		/// </summary>
		/// <param name="thisEntity">The Entity to reset.</param>
		/// <param name="registry">The EntityRegistry in which thisEntity is in.</param>
		void reset(Entity const thisEntity, Scene& scene) const;

	public:
		static Step parse_step(String const& string);

	private:
		step_key_t compile_key(size_t const entityIndex, size_t const componentIndex, size_t const variableIndex) const;

		step_value_t compile_value(size_t const valueIndex, StepFlags const flags) const;

		void perform_step(step_key_t const key, step_time_t const time, step_value_t const value, Entity const thisEntity, Scene& scene) const;

		void perform_step(Step const& step, Entity const thisEntity, Scene& scene) const;
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}