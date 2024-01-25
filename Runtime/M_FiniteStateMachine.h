#pragma once

#include "M_Object.h"
#include "M_ISerializable.h"
#include "M_Dynamic.h"
#include "M_Register.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace minty
{
	/// <summary>
	/// An equality condition.
	/// </summary>
	enum class Conditional
	{
		Equal,

		NotEqual,

		GreaterThan,

		GreaterThanOrEqualTo,

		LessThan,

		LessThanOrEqualTo,
	};

	String to_string(Conditional const value);
	Conditional from_string_conditional(String const& value);

	class FSM
		: public Object
	{
	public:
		/// <summary>
		/// Holds the data for the FSM.
		/// </summary>
		class Scope
			: public Object
		{
		private:
			std::unordered_map<String, ID> _ids;

			std::vector<int> _values;

		public:
			/// <summary>
			/// Creates an empty Scope.
			/// </summary>
			Scope();

			/// <summary>
			/// Checks if this Scope contains a variable with the given name.
			/// </summary>
			/// <param name="key"></param>
			/// <returns></returns>
			bool contains(String const& key) const;

			/// <summary>
			/// Creates a new variable within this Scope.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <param name="value">The initial value of the variable.</param>
			/// <returns>The ID of the variable.</returns>
			ID emplace(String const& key, int const value);

			/// <summary>
			/// Gets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The value of the variable.</returns>
			int get(String const& key) const;

			/// <summary>
			/// Gets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <returns>The value of the variable.</returns>
			int get(ID const key) const;

			/// <summary>
			/// Gets the ID of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The ID of the variable.</returns>
			ID get_id(String const& key) const;

			/// <summary>
			/// Gets the name of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <returns>The name of the variable.</returns>
			String get_name(ID const key) const;

			/// <summary>
			/// Finds the ID of the variable with the given name.
			/// </summary>
			/// <param name="key">The name to search for.</param>
			/// <returns>The ID, or ERROR_ID if none found.</returns>
			ID find(String const& key) const;

			/// <summary>
			/// Sets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(ID const key, int const value);

			/// <summary>
			/// Sets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(String const& key, int const value);

			/// <summary>
			/// Erases all data from the Scope.
			/// </summary>
			void clear();

			/// <summary>
			/// Sets all values to their defaults.
			/// </summary>
			void reset();

			void serialize(Writer& writer) const override;
			void deserialize(Reader const& reader) override;
		};

		/// <summary>
		/// A single condition for a Transition. 
		/// 
		/// The condition evaluates as: value of <variable with variable ID> <conditional> <value>.
		/// </summary>
		class Condition
			: public Object
		{
		private:
			/// <summary>
			/// The ID of the variable to be used in the condition.
			/// </summary>
			ID _variableId;

			/// <summary>
			/// The conditional type to be used.
			/// </summary>
			Conditional _conditional;

			/// <summary>
			/// The value that the variable is being compared against.
			/// </summary>
			int _value;

		public:
			/// <summary>
			/// Creates an empty condition.
			/// </summary>
			Condition();

			/// <summary>
			/// Parses the given pretty string into a Condition. Must be in the format: variableName conditional value
			/// </summary>
			/// <param name="value">Must be in the format: variableName conditional value</param>
			Condition(String const& value, FSM const& fsm);

			/// <summary>
			/// Creates a new Condition using the given variable ID, conditional type, and value.
			/// </summary>
			/// <param name="variableId">The variable to be referenced when evaluating this Condition.</param>
			/// <param name="conditional">The conditional type to be used when evaluating this Condition.</param>
			/// <param name="value">The value to use when comparing to the value of the variable with the given ID.</param>
			Condition(ID const variableId, Conditional const conditional, int const value);

			/// <summary>
			/// Evaluates this Condition using the given Scope.
			/// </summary>
			/// <param name="scope"></param>
			/// <returns>True when the condition has been met.</returns>
			bool evaluate(Scope const& scope) const;

			String to_pretty_string(FSM const& fsm) const;

			void serialize(Writer& writer) const override;
			void deserialize(Reader const& reader) override;
		};

		/// <summary>
		/// A transition from one State to another State.
		/// </summary>
		class Transition
			: public Object
		{
		private:
			/// <summary>
			/// The ID of the State that this transition leads to.
			/// </summary>
			ID _stateId;

			/// <summary>
			/// A list of conditions that must all be met for the transition to occur.
			/// </summary>
			std::vector<Condition> _conditions;

		public:
			/// <summary>
			/// Creates an empty transition.
			/// </summary>
			Transition();

			/// <summary>
			/// Creates a new Transition to the State with the given ID using the given Conditions.
			/// </summary>
			/// <param name="stateId"></param>
			/// <param name="conditions"></param>
			Transition(ID const stateId, std::vector<Condition> const& conditions);

			/// <summary>
			/// Gets the State ID.
			/// </summary>
			/// <returns>The ID to the destination State.</returns>
			ID get_state() const;

			/// <summary>
			/// Adds a condition to this Transition.
			/// </summary>
			/// <param name="condition"></param>
			void emplace_condition(Condition const& condition);

			/// <summary>
			/// Evaluates this Transition based on the given Scope.
			/// </summary>
			/// <param name="scope">The scope to use to evaluate the conditions within this Transition.</param>
			/// <returns>True when all conditions are met with the given Scope, otherwise false.</returns>
			bool evaluate(Scope const& scope) const;

			void serialize(Writer& writer) const override;
			void deserialize(Reader const& reader) override;
		};

		/// <summary>
		/// A state within the FSM that can hold some data.
		/// </summary>
		class State
			: public Object
		{
		private:
			Dynamic _value;

			std::vector<Transition> _transitions;
		public:
			/// <summary>
			/// Creates an empty State.
			/// </summary>
			State();

			/// <summary>
			/// Creates a new State with the given name and value.
			/// </summary>
			/// <param name="name">The name of the State.</param>
			/// <param name="value">The value this State holds.</param>
			State(Dynamic const& value);

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			Dynamic& get_value();

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			Dynamic const& get_value() const;

			/// <summary>
			/// Adds a transition to this State.
			/// </summary>
			/// <param name="transition"></param>
			void emplace_transition(Transition const& transition);

			/// <summary>
			/// Evaluates the given Scope. If the conditions are met to transition to a new State, that State's new ID is returned.
			/// </summary>
			/// <param name="scope">The FSM scope.</param>
			/// <returns>The new State's ID, or ERROR_ID if no transition occured.</returns>
			ID evaluate(Scope const& scope) const;

			void serialize(Writer& writer) const override;
			void deserialize(Reader const& reader) override;
		};

	private:
		// the scope
		Scope _scope;

		// a collection of all the states
		Register<State> _states;

		// the id of the starting state
		ID _startingStateId;

		// the id of the currently active state
		ID _currentStateId;
	public:
		/// <summary>
		/// Creates an empty Finite State Machine.
		/// </summary>
		FSM();

		~FSM();

		/// <summary>
		/// Creates a State within the FSM.
		/// </summary>
		/// <param name="name">The name of the new State.</param>
		/// <param name="value">The value of the new State.</param>
		/// <returns>The ID to the new State.</returns>
		ID create_state(String const& name, Dynamic const& value);

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		FSM::State& get_state(ID const id);

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		FSM::State const& get_state(ID const id) const;

		/// <summary>
		/// Gets the name of the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the State.</param>
		/// <returns>The name of the State.</returns>
		String get_state_name(ID const id) const;

		/// <summary>
		/// Finds the State with the given name.
		/// </summary>
		/// <param name="name">The name of the State.</param>
		/// <returns>The ID of the State, or ERROR_ID if none found.</returns>
		ID find_state(String const& name) const;

		/// <summary>
		/// Sets the starting State to the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the new State, or ERROR_ID to set to no State.</param>
		void set_starting_state(ID const id);

		/// <summary>
		/// Sets the starting State to the State with the given name.
		/// </summary>
		/// <param name="id">The name of the new State.</param>
		void set_starting_state(String const& name);

		/// <summary>
		/// Sets the current State to the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the new State, or ERROR_ID to set to no State.</param>
		void set_current_state(ID const id);

		/// <summary>
		/// Sets the current State to the State with the given name.
		/// </summary>
		/// <param name="id">The name of the new State.</param>
		void set_current_state(String const& name);

		/// <summary>
		/// Checks if there is a current state selected.
		/// </summary>
		/// <returns></returns>
		bool has_current_state() const;

		/// <summary>
		/// Gets the current State.
		/// </summary>
		/// <returns></returns>
		State& get_current_state();

		/// <summary>
		/// Gets the current State.
		/// </summary>
		/// <returns></returns>
		State const& get_current_state() const;

		/// <summary>
		/// Gets the value from the current State.
		/// </summary>
		/// <returns></returns>
		Dynamic& get_current_value();

		/// <summary>
		/// Gets the value from the current State.
		/// </summary>
		/// <returns></returns>
		Dynamic const& get_current_value() const;

		/// <summary>
		/// Creates a new variable with the given name and initial value.
		/// </summary>
		/// <param name="name">The name of the new variable.</param>
		/// <param name="initialValue">The starting value of the variable.</param>
		/// <returns>The ID of the new variable.</returns>
		ID create_variable(String const& name, int const initialValue);

		/// <summary>
		/// Sets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(ID const id, int const value);

		/// <summary>
		/// Sets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(String const& name, int const value);

		/// <summary>
		/// Gets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		int get_variable(ID const id) const;

		/// <summary>
		/// Gets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		int get_variable(String const& name) const;
		
		/// <summary>
		/// Gets the name of the variable with the given ID.
		/// </summary>
		/// <param name="id">The ID of the variable.</param>
		/// <returns>The name of the variable.</returns>
		String get_variable_name(ID const id) const;

		/// <summary>
		/// Finds the variable ID with the given name.
		/// </summary>
		/// <param name="name">The name of the variable.</param>
		/// <returns>The ID of the variable, or ERROR_ID if none found.</returns>
		ID find_variable(String const& name) const;

		/// <summary>
		/// Evaluates the FSM. 
		/// The current state will attempt to transition to another State based on its Transitions and values stored within this FSM.
		/// </summary>
		/// <returns>0 on a successful evaluation, 1 on no transition, 2 if there is no current state, or 3 if there was an infinite loop while transitioning.</returns>
		int evaluate();

		/// <summary>
		/// Resets the FSM.
		/// </summary>
		void reset();

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;
	};
}