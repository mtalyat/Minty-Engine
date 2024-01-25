#pragma once

#include "M_Object.h"
#include "M_ISerializable.h"
#include "M_Dynamic.h"
#include "M_EntityRegistry.h"
#include <vector>

namespace minty
{
	struct AnimationBuilder;

	/*
	
	Animations can be ran. They can edit values of other components.

	Name of animation
	Data: (animation states)
		Time:
			Name of component to edit
			Name of value to edit (use offsetof to edit data directly)
			Value to set (Dynamic?)
	
	*/

	/// <summary>
	/// Holds data for an animation that can be ran.
	/// </summary>
	class Animation
		: public Object
	{
	public:
		constexpr static size_t const MAX_STEP = 0b11;
		constexpr static size_t const MAX_INDEX = 0xff;
		constexpr static size_t const MAX_VALUE_INDEX = 0x3fffffff;

		enum class StepType
		{
			/// <summary>
			/// Perform a step as normal.
			/// </summary>
			Normal = 0b00,
			/// <summary>
			/// Add the Component to the Entity, if it does not exist.
			/// </summary>
			Add = 0b01,
			/// <summary>
			/// Remove the Component from the Entity, if it exists.
			/// </summary>
			Remove = 0b10,
			/// <summary>
			/// Ignore this step.
			/// </summary>
			Ignore = 0b11,
		};

		/// <summary>
		/// Holds all of the indices required for a step, to different component values.
		/// </summary>
		struct Step
		{
			StepType type = StepType::Ignore;
			size_t entityIndex = MAX_INDEX;
			size_t componentIndex = MAX_INDEX;
			size_t offsetIndex = MAX_INDEX;
			size_t sizeIndex = MAX_INDEX;
			size_t valueIndex = MAX_VALUE_INDEX;
		};

	private:
		/// <summary>
		/// The amount of time this Animation runs for, in seconds.
		/// </summary>
		float _length;

		/// <summary>
		/// The animation loops.
		/// </summary>
		bool _loops;

		/// <summary>
		/// A list of all Entities being affected by this Animation.
		/// </summary>
		std::vector<String> _entities;

		/// <summary>
		/// A list of all Components being affected by this Animation.
		/// </summary>
		std::vector<String> _components;

		/// <summary>
		/// A list of all offset/sizes being set by this Animation.
		/// 
		/// Each offset/size corresponds to somewhere within one of the components being edited.
		/// </summary>
		std::vector<size_t> _sizes;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Dynamic> _values;

		/// <summary>
		/// The compilation of steps within this Animation.
		/// [Step type: 2 bits][Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 30 bits]
		/// </summary>
		std::vector<std::pair<float, uint64_t>> _steps;

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
		/// <param name="registry">The EntityRegistry in which this Animation is acting in.</param>
		/// <returns>True when the animation has completed, otherwise false.</returns>
		bool animate(float& time, float const elapsedTime, size_t& index, Entity const thisEntity, EntityRegistry& registry) const;

	private:
		static uint64_t compile_step(Step const& step);

		static void uncompile_step(size_t const value, Step& step);

	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}