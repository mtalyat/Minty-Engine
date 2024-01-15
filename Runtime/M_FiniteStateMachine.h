#pragma once

#include "M_Object.h"
#include "M_Dynamic.h"
#include "M_Register.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace minty
{
	/// <summary>
	/// A Finite State Machine.
	/// </summary>
	template<typename ValueType>
	class FSM
		: public Object
	{
	public:
		typedef int VariableType;

	public:
		/// <summary>
		/// Holds the data for the FSM.
		/// </summary>
		class Scope
		{
		private:
			std::unordered_map<String, ID> _ids;

			std::vector<VariableType> _values;

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
			ID emplace(String const& key, VariableType const value);

			/// <summary>
			/// Gets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The value of the variable.</returns>
			VariableType get(String const& key) const;

			/// <summary>
			/// Gets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <returns>The value of the variable.</returns>
			VariableType get(ID const key) const;

			/// <summary>
			/// Gets the ID of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <returns>The ID of the variable.</returns>
			ID get_id(String const& key) const;

			/// <summary>
			/// Sets the value of the variable with the given ID.
			/// </summary>
			/// <param name="key">The ID of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(ID const key, VariableType const value);

			/// <summary>
			/// Sets the value of the variable with the given name.
			/// </summary>
			/// <param name="key">The name of the variable.</param>
			/// <param name="value">The new value of the variable.</param>
			void set(String const& key, VariableType const value);
		};

		/// <summary>
		/// An equality condition for a transition.
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
			ID _variableId;

			/// <summary>
			/// The conditional type to be used.
			/// </summary>
			Conditional _conditional;

			/// <summary>
			/// The value that the variable is being compared against.
			/// </summary>
			VariableType _value;

		public:
			/// <summary>
			/// Creates a new Condition using the given variable ID, conditional type, and value.
			/// </summary>
			/// <param name="variableId">The variable to be referenced when evaluating this Condition.</param>
			/// <param name="conditional">The conditional type to be used when evaluating this Condition.</param>
			/// <param name="value">The value to use when comparing to the value of the variable with the given ID.</param>
			Condition(ID const variableId, Conditional const conditional, VariableType const value);

			/// <summary>
			/// Evaluates this Condition using the given Scope.
			/// </summary>
			/// <param name="scope"></param>
			/// <returns>True when the condition has been met.</returns>
			bool evaluate(Scope const& scope) const;
		};

		/// <summary>
		/// A transition from one State to another State.
		/// </summary>
		class Transition
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
		};

		/// <summary>
		/// A state within the FSM that can hold some data.
		/// </summary>
		class State
		{
		private:
			String _name;

			ValueType _value;

			std::vector<Transition> _transitions;
		public:
			/// <summary>
			/// Creates a new State with the given name and value.
			/// </summary>
			/// <param name="name">The name of the State.</param>
			/// <param name="value">The value this State holds.</param>
			State(String const& name, ValueType const& value);

			/// <summary>
			/// Gets the name of this State.
			/// </summary>
			/// <returns></returns>
			String const& get_name() const;

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			ValueType& get_value();

			/// <summary>
			/// Gets the value of this State.
			/// </summary>
			/// <returns></returns>
			ValueType const& get_value() const;

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
		};

	private:
		// the scope
		Scope _scope;

		// a collection of all the states
		Register<State> _states;

		// the id of the currently active state
		ID _currentStateId;
	public:
		/// <summary>
		/// Creates an empty Finite State Machine.
		/// </summary>
		FSM();

		/// <summary>
		/// Creates a State within the FSM.
		/// </summary>
		/// <param name="name">The name of the new State.</param>
		/// <param name="value">The value of the new State.</param>
		/// <returns>The ID to the new State.</returns>
		ID create_state(String const& name, ValueType const& value);

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		State& get_state(ID const id);

		/// <summary>
		/// Gets the State with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		State const& get_state(ID const id) const;

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
		ValueType& get_current_value();

		/// <summary>
		/// Gets the value from the current State.
		/// </summary>
		/// <returns></returns>
		ValueType const& get_current_value() const;

		/// <summary>
		/// Creates a new variable with the given name and initial value.
		/// </summary>
		/// <param name="name">The name of the new variable.</param>
		/// <param name="initialValue">The starting value of the variable.</param>
		/// <returns>The ID of the new variable.</returns>
		ID create_variable(String const& name, VariableType const initialValue);

		/// <summary>
		/// Sets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(ID const id, VariableType const value);

		/// <summary>
		/// Sets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="value"></param>
		void set_variable(String const& name, VariableType const value);

		/// <summary>
		/// Gets the value of the variable with the given ID.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		VariableType get_variable(ID const id) const;

		/// <summary>
		/// Gets the value of the variable with the given name.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		VariableType get_variable(String const& name) const;

		/// <summary>
		/// Evaluates the FSM. 
		/// The current state will attempt to transition to another State based on its Transitions and values stored within this FSM.
		/// </summary>
		/// <returns>0 on a successful evaluation, 1 if there is no current state, or 2 if there was an infinite loop while transitioning.</returns>
		int evaluate();
	};

	template<typename ValueType>
	FSM<ValueType>::Scope::Scope()
		: _ids()
		, _values()
	{}
	template<typename ValueType>
	bool FSM<ValueType>::Scope::contains(String const& key) const
	{
		return _ids.contains(key);
	}
	template<typename ValueType>
	ID FSM<ValueType>::Scope::emplace(String const& key, VariableType const value)
	{
		// get id
		ID id = static_cast<ID>(_values.size());

		// add to ids
		_ids.emplace(key, id);

		// add to values
		_values.push_back(value);

		return id;
	}
	template<typename ValueType>
	FSM<ValueType>::VariableType FSM<ValueType>::Scope::get(String const& key) const
	{
		return _values.at(_ids.at(key));
	}
	template<typename ValueType>
	FSM<ValueType>::VariableType FSM<ValueType>::Scope::get(ID const key) const
	{
		return _values.at(key);
	}
	template<typename ValueType>
	ID FSM<ValueType>::Scope::get_id(String const& key) const
	{
		return _ids.at(key);
	}
	template<typename ValueType>
	void FSM<ValueType>::Scope::set(ID const key, VariableType const value)
	{
		_values[key] = value;
	}
	template<typename ValueType>
	void FSM<ValueType>::Scope::set(String const& key, VariableType const value)
	{
		_values[_ids.at(key)] = value;
	}

	template<typename ValueType>
	FSM<ValueType>::Condition::Condition(ID const variableId, Conditional const conditional, VariableType const value)
		: _variableId(variableId)
		, _conditional(conditional)
		, _value(value)
	{}
	template<typename ValueType>
	bool FSM<ValueType>::Condition::evaluate(Scope const& scope) const
	{
		// get value of variable from the scope
		VariableType value = scope.at(_variableId);

		// evaluate
		switch (_conditional)
		{
		case Conditional::Equal: return value == _value;
		case Conditional::NotEqual: return value != _value;
		case Conditional::GreaterThan: return value > _value;
		case Conditional::GreaterThanOrEqualTo: return value >= _value;
		case Conditional::LessThan: return value < _value;
		case Conditional::LessThanOrEqualTo: return value <= _value;
		default: return false; // conditional DNE
		}
	}

	template<typename ValueType>
	FSM<ValueType>::Transition::Transition(ID const stateId, std::vector<Condition> const& conditions)
		: _stateId(stateId)
		, _conditions(conditions)
	{}
	template<typename ValueType>
	ID FSM<ValueType>::Transition::get_state() const
	{
		return _stateId;
	}
	template<typename ValueType>
	void FSM<ValueType>::Transition::emplace_condition(Condition const& condition)
	{
		_conditions.push_back(condition);
	}
	template<typename ValueType>
	bool FSM<ValueType>::Transition::evaluate(Scope const& scope) const
	{
		// check each condition
		for (Condition const& condition : _conditions)
		{
			if (!condition.evaluate(scope))
			{
				// condition did not pass, so this transition will not happen
				return false;
			}
		}

		// all conditions passed
		return true;
	}

	template<typename ValueType>
	FSM<ValueType>::State::State(String const& name, ValueType const& value)
		: _name(name)
		, _value(value)
		, _transitions()
	{}
	template<typename ValueType>
	String const& FSM<ValueType>::State::get_name() const
	{
		return _name;
	}
	template<typename ValueType>
	ValueType& FSM<ValueType>::State::get_value()
	{
		return _value;
	}
	template<typename ValueType>
	ValueType const& FSM<ValueType>::State::get_value() const
	{
		return _value;
	}
	template<typename ValueType>
	void FSM<ValueType>::State::emplace_transition(Transition const& transition)
	{
		_transitions.push_back(transition);
	}
	template<typename ValueType>
	ID FSM<ValueType>::State::evaluate(Scope const& scope) const
	{
		// check each transition, if it evaluates to true, then move there
		for (Transition const& transition : _transitions)
		{
			if (transition.evaluate(scope))
			{
				return transition.get_state();
			}
		}

		// no transitions evaluated
		return ERROR_ID;
	}

	template<typename ValueType>
	FSM<ValueType>::FSM()
		: _scope()
		, _states()
		, _currentStateId(ERROR_ID)
	{}
	template<typename ValueType>
	ID FSM<ValueType>::create_state(String const& name, ValueType const& value)
	{
		return _states.emplace(name, State(name, value));
	}
	template<typename ValueType>
	FSM<ValueType>::State& FSM<ValueType>::get_state(ID const id)
	{
		return _states.at(id);
	}
	template<typename ValueType>
	FSM<ValueType>::State const& FSM<ValueType>::get_state(ID const id) const
	{
		return _states.at(id);
	}
	template<typename ValueType>
	void FSM<ValueType>::set_current_state(ID const id)
	{
		_currentStateId = id;
	}
	template<typename ValueType>
	void FSM<ValueType>::set_current_state(String const& name)
	{
		_currentStateId = _states.get_id(name);
	}
	template<typename ValueType>
	FSM<ValueType>::State& FSM<ValueType>::get_current_state()
	{
		return _states.at(_currentStateId);
	}
	template<typename ValueType>
	FSM<ValueType>::State const& FSM<ValueType>::get_current_state() const
	{
		return _states.at(_currentStateId);
	}
	template<typename ValueType>
	ValueType& FSM<ValueType>::get_current_value()
	{
		return _states.at(_currentStateId).get_value();
	}
	template<typename ValueType>
	ValueType const& FSM<ValueType>::get_current_value() const
	{
		return _states.at(_currentStateId).get_value();
	}
	template<typename ValueType>
	ID FSM<ValueType>::create_variable(String const& name, VariableType const initialValue)
	{
		return _scope.emplace(name, initialValue);
	}
	template<typename ValueType>
	void FSM<ValueType>::set_variable(ID const id, VariableType const value)
	{
		_scope.set(id, value);
	}
	template<typename ValueType>
	void FSM<ValueType>::set_variable(String const& name, VariableType const value)
	{
		_scope.set(id, value);
	}
	template<typename ValueType>
	FSM<ValueType>::VariableType FSM<ValueType>::get_variable(ID const id) const
	{
		return _scope.get(id);
	}
	template<typename ValueType>
	FSM<ValueType>::VariableType FSM<ValueType>::get_variable(String const& name) const
	{
		return _scope.get(name);
	}
	template<typename ValueType>
	int FSM<ValueType>::evaluate()
	{
		// if no currently selected state, do nothing
		if (_currentStateId == ERROR_ID)
		{
			return 1;
		}

		// to check for infinite loops, keep a set of states that have been traversed
		std::unordered_set<ID> _visitedStates;

		ID state = _currentStateId;
		ID next = state;

		// keep looping until no more transitions have been made
		while (next != ERROR_ID)
		{
			// move to next state
			state = next;
			next = get_state(next).evaluate(_scope);

			// if we have visited this state, then an infinite loop has occured
			if (!_visitedStates.emplace(next)->second)
			{
				// could not add: already been visited
				return 2;
			}
		}

		// successfully transitioned to the next state
		set_current_state(state);

		return 0;
	}
}