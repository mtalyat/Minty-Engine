#pragma once

#include "Minty/Asset/Asset.h"
#include "Minty/Entity/Entity.h"
#include "Minty/Serialize/Serializable.h"
#include <map>
#include <vector>
#include <unordered_map>

namespace Minty
{
	class Scene;

	enum AnimationStepFlags
	{
		ANIMATION_STEP_FLAGS_NONE = 0b0000,
		ANIMATION_STEP_FLAGS_ADD_REMOVE = 0b0001,
		ANIMATION_STEP_FLAGS_ALL = 0b0001,
	};

	/// <summary>
	/// Holds all of the indices required for a step, to different component values.
	/// </summary>
	struct AnimationStep
	{
		Size entityIndex;
		Size componentIndex;
		Size variableIndex;
		Size timeIndex;
		Size valueIndex;
		AnimationStepFlags flags;
	};

	/// <summary>
	/// Holds data to create a new Animation.
	/// </summary>
	struct AnimationBuilder
	{
		UUID id;

		/// <summary>
		/// The amount of time this Animation runs for, in seconds.
		/// </summary>
		Float length;

		/// <summary>
		/// The animation will loop when it ends.
		/// </summary>
		Bool loops;

		/// <summary>
		/// A list of all Entities being affected by this Animation.
		/// </summary>
		std::vector<String> entities;

		/// <summary>
		/// A list of all Components being edited by this Animation.
		/// </summary>
		std::vector<String> components;

		/// <summary>
		/// A list of all variable names being set by this Animation.
		/// </summary>
		std::vector<String> variables;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Node> values;

		/// <summary>
		/// The steps within this Animation.
		/// </summary>
		//std::vector<std::pair<Float, AnimationStep>> steps;
		std::map<Float, std::vector<AnimationStep>> steps;

		/// <summary>
		/// The steps taken when resetting this Animation.
		/// </summary>
		std::vector<AnimationStep> resetSteps;
	};

	/// <summary>
	/// Holds data for an animation that can be ran.
	/// </summary>
	class Animation
		: public Asset, public Serializable
	{
	private:
		enum Flags : Int
		{
			ANIMATION_FLAGS_NONE = 0,
			ANIMATION_FLAGS_LOOPS = 0b00000001,
			ANIMATION_FLAGS_ALL = 0b00000001,
		};

	private:
		constexpr static Int ENTITY_OFFSET = 16;
		constexpr static Int COMPONENT_OFFSET = 8;
		constexpr static Int VARIABLE_OFFSET = 0;
		constexpr static Int TIME_OFFSET = 0;
		constexpr static Int VALUE_OFFSET = 4;
		constexpr static Int FLAGS_OFFSET = 0;
		constexpr static Size MAX_ENTITY_INDEX = 0xff;
		constexpr static Size MAX_COMPONENT_INDEX = 0xff;
		constexpr static Size MAX_VARIABLE_INDEX = 0xff;
		constexpr static Size MAX_TIME_INDEX = 0xffffffff;
		constexpr static Size MAX_VALUE_INDEX = 0xfffffff;
		constexpr static Size MAX_FLAGS_INDEX = 0xf;
		typedef UInt step_key_t;
		typedef UInt step_time_t;
		typedef UInt step_value_t;
		constexpr static step_key_t INVALID_STEP_KEY = (MAX_ENTITY_INDEX << ENTITY_OFFSET) | (MAX_COMPONENT_INDEX << COMPONENT_OFFSET) | (MAX_VARIABLE_INDEX | VARIABLE_OFFSET);
		constexpr static step_time_t INVALID_STEP_TIME = (MAX_TIME_INDEX << TIME_OFFSET);
		constexpr static step_value_t INVALID_STEP_VALUE = (MAX_VALUE_INDEX << VALUE_OFFSET) | (MAX_FLAGS_INDEX << FLAGS_OFFSET);

	public:
		typedef UInt Index;
	private:
		/// <summary>
		/// The amount of time this Animation runs for, in seconds.
		/// </summary>
		Float m_length;

		Flags m_flags;

		/// <summary>
		/// A list of all Entities being affected by this Animation.
		/// </summary>
		std::vector<String> m_entities;

		/// <summary>
		/// A list of all Components being affected by this Animation.
		/// </summary>
		std::vector<String> m_components;

		/// <summary>
		/// A list of all variable names being used by this Animation.
		/// </summary>
		std::vector<String> m_variables;

		/// <summary>
		/// List of all time intervals in this Animation.
		/// </summary>
		std::vector<Float> m_times;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Node> m_values;

		/// <summary>
		/// The compilation of steps within this Animation.
		/// [Entity index: 8 bits][Component index: 8 bits][Offset index: 8 bits][Time index: 18 bits][Value index: 18 bits][Flags: 4 bits]
		/// 
		/// Key: The combined IDs of Entity, Component, Variable
		/// Value Key: Time ID
		/// Value Value: Value and Flags
		/// </summary>
		std::unordered_map<step_key_t, std::unordered_map<step_time_t, step_value_t>> m_steps;

		/// <summary>
		/// The steps taken when the animation is reset.
		/// </summary>
		std::unordered_map<step_key_t, std::vector<step_value_t>> m_reset;

	public:
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
		Bool loops() const;

		/// <summary>
		/// Performs an animation on an Entity.
		/// </summary>
		/// <param name="time">The current time. This will be updated.</param>
		/// <param name="elapsedTime">The time that has elapsed over the last frame.</param>
		/// <param name="index">The index of the next step to perform.</param>
		/// <param name="thisEntity">The entity in which this Animation is being acted upon.</param>
		/// <param name="registry">The EntityRegistry in which thisEntity is in.</param>
		/// <returns>True when the animation has completed, otherwise false.</returns>
		Bool animate(Float& time, Float const elapsedTime, Index& index, Entity const thisEntity, Scene& scene) const;

		/// <summary>
		/// Performs a reset on an Entity.
		/// </summary>
		/// <param name="thisEntity">The Entity to reset.</param>
		/// <param name="registry">The EntityRegistry in which thisEntity is in.</param>
		void reset(Entity const thisEntity, Scene& scene) const;

	public:
		AssetType get_type() const override { return AssetType::Animation; }

	public:
		static AnimationStep parse_step(String const& string);

	private:
		step_key_t compile_key(Size const entityIndex, Size const componentIndex, Size const variableIndex) const;

		step_value_t compile_value(Size const valueIndex, AnimationStepFlags const flags) const;

		void perform_step(step_key_t const key, step_time_t const time, step_value_t const value, Entity const thisEntity, Scene& scene) const;

		void perform_step(AnimationStep const& step, Entity const thisEntity, Scene& scene) const;
	public:
		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;

	public:
		static Owner<Animation> create(AnimationBuilder const& builder = {}) { return Owner<Animation>(builder); }
	};
}