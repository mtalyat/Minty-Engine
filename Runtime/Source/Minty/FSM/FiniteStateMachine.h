#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include "Minty/Core/UUID.h"
#include "Minty/Data/DynamicContainer.h"
#include "Minty/Data/Lookup.h"
#include "Minty/Data/Variable.h"
#include "Minty/FSM/Conditional.h"
#include "Minty/Serialize/Serializable.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace Minty
{
	class FSM
		: public Serializable
	{
	public:
		/// <summary>
		/// Holds the data for the FSM.
		/// </summary>
		class Scope
			: public Serializable
		{
		private:
			Lookup<Int> m_values;

		public:
			/// <summary>
			/// Creates an empty Scope.
			/// </summary>
			Scope()
				: m_values()
			{}

			/// <summary>
			/// Checks if this Scope contains a variable with the given name.
			/// </summary>
			/// <param name="key"></param>
			/// <returns></returns>
			Bool contains(String const& key) const { return m_values.contains(key); }

			/// <summary>
			/// Creates a new variable within this Scope.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <param name="value">The initial value of the variable.</param>
			/// <returns>The ID of the variable.</returns>
			UUID emplace(String const& key, Int const value);

			/// <summary>
			/// Gets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The value of the variable.</returns>
			Int get(String const& key) const { return m_values.at(key); }

			/// <summary>
			/// Gets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <returns>The value of the variable.</returns>
			Int get(UUID const key) const { return m_values.at(key); }

			/// <summary>
			/// Gets the ID of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The ID of the variable.</returns>
			UUID get_id(String const& key) const { return m_values.get_id(key); }

			/// <summary>
			/// Gets the name of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <returns>The name of the variable.</returns>
			String get_name(UUID const key) const { return m_values.get_name(key); }

			/// <summary>
			/// Finds the ID of the variable with the given name.
			/// </summary>
			/// <param name="key">The name to search for.</param>
			/// <returns>The ID, or ERROR_ID if none found.</returns>
			UUID find(String const& key) const;

			/// <summary>
			/// Sets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(UUID const key, Int const value) { m_values.at(key) = value; }

			/// <summary>
			/// Sets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(String const& key, Int const value) { m_values.at(key) = value; }

			/// <summary>
			/// Erases all data from the Scope.
			/// </summary>
			void clear() { m_values.clear(); }

			/// <summary>
			/// Sets all values to their defaults.
			/// </summary>
			void reset();

			void serialize(Writer& writer) const override;
			void deserialize(Reader& reader) override;
		};

		/// <summary>
		/// A single condition for a Transition. 
		/// 
		/// The condition evaluates as: value of <variable with variable ID> <conditional> <value>.
		/// </summary>
		class Condition
		{
		private:
			/// <summary>
			/// The ID of the variable to be used in the condition.
			/// </summary>
			UUID m_variableId;

			/// <summary>
			/// The conditional type to be used.
			/// </summary>
			Conditional m_conditional;

			/// <summary>
			/// The value that the variable is being compared against.
			/// </summary>
			Int m_value;

		public:
			/// <summary>
			/// Creates an empty condition.
			/// </summary>
			Condition();

			/// <summary>
			/// Parses the given pretty string Into a Condition. Must be in the format: variableName conditional value
			/// </summary>
			/// <param name="value">Must be in the format: variableName conditional value</param>
			Condition(String const& value, FSM const& fsm);

			/// <summary>
			/// Creates a new Condition using the given variable ID, conditional type, and value.
			/// </summary>
			/// <param name="variableId">The variable to be referenced when evaluating this Condition.</param>
			/// <param name="conditional">The conditional type to be used when evaluating this Condition.</param>
			/// <param name="value">The value to use when comparing to the value of the variable with the given ID.</param>
			Condition(UUID const variableId, Conditional const conditional, Int const value);

			/// <summary>
			/// Evaluates this Condition using the given Scope.
			/// </summary>
			/// <param name="scope"></param>
			/// <returns>True when the condition has been met.</returns>
			Bool evaluate(Scope const& scope) const;

			String to_pretty_string(FSM const& fsm) const;
		};

		/// <summary>
		/// A transition from one State to another State.
		/// </summary>
		class Transition
			: public Serializable
		{
		private:
			/// <summary>
			/// The ID of the State that this transition leads to.
			/// </summary>
			UUID m_stateId;

			/// <summary>
			/// A list of conditions that must all be met for the transition to occur.
			/// </summary>
			std::vector<Condition> m_conditions;

		public:
			/// <summary>
			/// Creates an empty transition.
			/// </summary>
			Transition()
				: m_stateId(INVALID_UUID)
				, m_conditions()
			{}

			/// <summary>
			/// Creates a new Transition to the State with the given ID using the given Conditions.
			/// </summary>
			/// <param name="stateId"></param>
			/// <param name="conditions"></param>
			Transition(UUID const stateId, std::vector<Condition> const& conditions)
				: m_stateId(stateId)
				, m_conditions(conditions)
			{}

			/// <summary>
			/// Gets the State ID.
			/// </summary>
			/// <returns>The ID to the destination State.</returns>
			UUID get_state() const { return m_stateId; }

			/// <summary>
			/// Adds a condition to this Transition.
			/// </summary>
			/// <param name="condition"></param>
			void emplace_condition(Condition const& condition) { m_conditions.push_back(condition); }

			/// <summary>
			/// Evaluates this Transition based on the given Scope.
			/// </summary>
			/// <param name="scope">The scope to use to evaluate the conditions within this Transition.</param>
			/// <returns>True when all conditions are met with the given Scope, otherwise false.</returns>
			Bool evaluate(Scope const& scope) const;

			void serialize(Writer& writer) const override;
			void deserialize(Reader& reader) override;
		};

		/// <summary>
		/// A state within the FSM that can hold some data.
		/// </summary>
		class State
			: public Serializable
		{
		private:
			Variable m_value;

			std::vector<Transition> m_transitions;
		public:
			/// <summary>
			/// Creates an empty State.
			/// </summary>
			State()
				: m_value()
				, m_transitions()
			{}

			/// <summary>
			/// Creates a new State with the given name and value.
			/// </summary>
			/// <param name="name">The name of the State.</param>
			/// <param name="value">The value this State holds.</param>
			State(Variable const& value)
				: m_value(value)
				, m_transitions()
			{}

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			Variable& get_value() { return m_value; }

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			Variable const& get_value() const { return m_value; }

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
			UUID evaluate(Scope const& scope) const;

			void serialize(Writer& writer) const override;
			void deserialize(Reader& reader) override;
		};

	private:
		// the scope
		Scope m_scope;

		// a collection of all the states
		Lookup<State> m_states;

		// the id of the starting state
		UUID m_startingStateId;

		// the id of the currently active state
		UUID m_currentStateId;
	public:
		/// <summary>
		/// Creates an empty Finite State Machine.
		/// </summary>
		FSM()
			: m_scope()
			, m_states()
			, m_startingStateId(INVALID_UUID) // start with the first State created, by default
			, m_currentStateId(INVALID_UUID)
		{}

		~FSM() {}

		/// <summary>
		/// Creates a State within the FSM.
		/// </summary>
		/// <param name="name">The name of the new State.</param>
		/// <param name="value">The value of the new State.</param>
		/// <returns>The ID to the new State.</returns>
		UUID create_state(String const& name, Variable const& value);

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		FSM::State& get_state(UUID const id)
		{
			return m_states.at(id);
		}

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		FSM::State const& get_state(UUID const id) const
		{
			return m_states.at(id);
		}

		/// <summary>
		/// Gets the name of the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the State.</param>
		/// <returns>The name of the State.</returns>
		String get_state_name(UUID const id) const
		{
			return m_states.get_name(id);
		}

		/// <summary>
		/// Finds the State with the given name.
		/// </summary>
		/// <param name="name">The name of the State.</param>
		/// <returns>The ID of the State, or ERROR_ID if none found.</returns>
		UUID find_state(String const& name) const;

		/// <summary>
		/// Sets the starting State to the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the new State, or ERROR_ID to set to no State.</param>
		void set_starting_state(UUID const id)
		{
			m_startingStateId = id;
		}

		/// <summary>
		/// Sets the starting State to the State with the given name.
		/// </summary>
		/// <param name="id">The name of the new State.</param>
		void set_starting_state(String const& name)
		{
			m_startingStateId = m_states.get_id(name);
		}

		/// <summary>
		/// Sets the current State to the State with the given ID.
		/// </summary>
		/// <param name="id">The ID of the new State, or ERROR_ID to set to no State.</param>
		void set_current_state(UUID const id)
		{
			m_currentStateId = id;
		}

		/// <summary>
		/// Sets the current State to the State with the given name.
		/// </summary>
		/// <param name="id">The name of the new State.</param>
		void set_current_state(String const& name)
		{
			m_currentStateId = m_states.get_id(name);
		}

		/// <summary>
		/// Checks if there is a current state selected.
		/// </summary>
		/// <returns></returns>
		Bool has_current_state() const
		{
			return m_currentStateId != INVALID_UUID;
		}

		/// <summary>
		/// Gets the current State.
		/// </summary>
		/// <returns></returns>
		State& get_current_state()
		{
			return m_states.at(m_currentStateId);
		}

		/// <summary>
		/// Gets the current State.
		/// </summary>
		/// <returns></returns>
		State const& get_current_state() const
		{
			return m_states.at(m_currentStateId);
		}

		/// <summary>
		/// Gets the value from the current State.
		/// </summary>
		/// <returns></returns>
		Variable& get_current_value()
		{
			return m_states.at(m_currentStateId).get_value();
		}

		/// <summary>
		/// Gets the value from the current State.
		/// </summary>
		/// <returns></returns>
		Variable const& get_current_value() const
		{
			return m_states.at(m_currentStateId).get_value();
		}

		/// <summary>
		/// Creates a new variable with the given name and initial value.
		/// </summary>
		/// <param name="name">The name of the new variable.</param>
		/// <param name="initialValue">The starting value of the variable.</param>
		/// <returns>The ID of the new variable.</returns>
		UUID create_variable(String const& name, Int const initialValue)
		{
			return m_scope.emplace(name, initialValue);
		}

		/// <summary>
		/// Sets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(UUID const id, Int const value)
		{
			m_scope.set(id, value);
		}

		/// <summary>
		/// Sets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(String const& name, Int const value)
		{
			m_scope.set(name, value);
		}

		/// <summary>
		/// Gets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Int get_variable(UUID const id) const
		{
			return m_scope.get(id);
		}

		/// <summary>
		/// Gets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Int get_variable(String const& name) const
		{
			return m_scope.get(name);
		}

		/// <summary>
		/// Gets the name of the variable with the given ID.
		/// </summary>
		/// <param name="id">The ID of the variable.</param>
		/// <returns>The name of the variable.</returns>
		String get_variable_name(UUID const id) const
		{
			return m_scope.get_name(id);
		}

		/// <summary>
		/// Finds the variable ID with the given name.
		/// </summary>
		/// <param name="name">The name of the variable.</param>
		/// <returns>The ID of the variable, or ERROR_ID if none found.</returns>
		UUID find_variable(String const& name) const
		{
			return m_scope.get_id(name);
		}

		/// <summary>
		/// Evaluates the FSM. 
		/// The current state will attempt to transition to another State based on its Transitions and values stored within this FSM.
		/// </summary>
		/// <returns>0 on a successful evaluation, 1 on no transition, 2 if there is no current state, or 3 if there was an infinite loop while transitioning.</returns>
		Int evaluate();

		/// <summary>
		/// Resets the FSM.
		/// </summary>
		void reset();

		void serialize(Writer& writer) const override;
		void deserialize(Reader& reader) override;
	};
}