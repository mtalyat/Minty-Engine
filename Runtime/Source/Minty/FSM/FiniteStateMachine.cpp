#include "pch.h"
#include "FiniteStateMachine.h"

#include "Minty/Core/Parse.h"

using namespace Minty;

UUID Minty::FSM::Scope::emplace(String const& key, Int const value)
{
	UUID id = UUID::create();
	m_values.emplace(id, key, value);
	return id;
}

UUID Minty::FSM::Scope::find(String const& key) const
{
	if (m_values.contains(key))
	{
		return m_values.at(key);
	}

	return INVALID_UUID;
}

void Minty::FSM::Scope::reset()
{
	for (Size i = 0; i < m_values.size(); i++)
	{
		m_values.at(i) = 0;
	}
}

void Minty::FSM::Scope::serialize(Writer& writer) const
{
	// write each name and value: ids are calculated automatically when deserialized
	for (auto const& [id, name, value] : m_values)
	{
		writer.write(name, value);
	}
}

void Minty::FSM::Scope::deserialize(Reader& reader)
{
	clear();

	// read all parts of the node as the variables
	String name;
	Int value;
	for (Size i = 0; i < reader.size(); i++)
	{
		// get name and value
		reader.read_name(i, name);
		reader.read(i, value);

		// add to scope
		emplace(name, value);
	}
}

Minty::FSM::Condition::Condition()
	: m_variableId(INVALID_UUID)
	, m_conditional()
	, m_value()
{}

Minty::FSM::Condition::Condition(String const& value, FSM const& fsm)
	: m_variableId(INVALID_UUID)
	, m_conditional()
	, m_value()
{
	// split into parts
	auto list = split(value);
	size_t size = list.size();

	// get UUID of variable from fsm
	m_variableId = fsm.find_variable(join(list, 0, size - 2));
	m_conditional = Parse::to_conditional(list.at(size - 2));
	m_value = Parse::to_int(list.at(size - 1));
}

Minty::FSM::Condition::Condition(UUID const variableId, Conditional const conditional, Int const value)
	: m_variableId(variableId)
	, m_conditional(conditional)
	, m_value(value)
{}

Bool Minty::FSM::Condition::evaluate(Scope const& scope) const
{
	//get value of variable from the scope
	Int value = scope.get(m_variableId);

	// evaluate
	switch (m_conditional)
	{
	case Conditional::Equal: return value == m_value;
	case Conditional::NotEqual: return value != m_value;
	case Conditional::GreaterThan: return value > m_value;
	case Conditional::GreaterThanOrEqualTo: return value >= m_value;
	case Conditional::LessThan: return value < m_value;
	case Conditional::LessThanOrEqualTo: return value <= m_value;
	default: return false; // conditional DNE
	}
}

String Minty::FSM::Condition::to_pretty_string(FSM const& fsm) const
{
	return std::format("{} {} {}", fsm.get_variable_name(m_variableId), to_string(m_conditional), m_value);
}

Bool Minty::FSM::Transition::evaluate(Scope const& scope) const
{
	// check each condition
	for (Condition const& condition : m_conditions)
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

void Minty::FSM::Transition::serialize(Writer& writer) const
{
	FSM const* fsm = static_cast<FSM const*>(writer.get_data_const());

	MINTY_ASSERT(fsm != nullptr);

	writer.write("state", m_stateId);

	writer.indent("conditions");

	for (auto const& condition : m_conditions)
	{
		writer.write(condition.to_pretty_string(*fsm));
	}

	writer.outdent();
}

void Minty::FSM::Transition::deserialize(Reader& reader)
{
	FSM const* fsm = static_cast<FSM const*>(reader.get_data());

	reader.read("state", m_stateId);

	if (reader.indent("conditions"))
	{
		String conditionString;
		for (Size i = 0; i < reader.size(); i++)
		{
			reader.read(i, conditionString);

			// split up pretty string
			std::vector<String> parts = split(conditionString);

			MINTY_ASSERT_FORMAT(parts.size() >= 3, "Error parsing Condition. At least 2 spaces needed to split the variable, conditional and value by: \"{}\".", conditionString);
			MINTY_ASSERT_FORMAT(parts.size() <= 3, "Error parsing Condition. At most 2 spaces needed to split the variable, conditional and value by: \"{}\".", conditionString);

			// get parts
			String variableName = parts.at(0);
			UUID variableId = fsm->find_variable(variableName);
			Conditional conditional = Parse::to_conditional(parts.at(1));
			Int value = Parse::to_int(parts.at(2));

			// add new condition
			emplace_condition(Condition(variableId, conditional, value));
		}

		reader.outdent();
	}
}

void Minty::FSM::State::emplace_transition(Transition const& transition)
{
	m_transitions.push_back(transition);
}

UUID Minty::FSM::State::evaluate(Scope const& scope) const
{
	// check each transition, if it evaluates to true, then move there
	for (Transition const& transition : m_transitions)
	{
		if (transition.evaluate(scope))
		{
			return transition.get_state();
		}
	}

	// no transitions evaluated
	return INVALID_UUID;
}

void Minty::FSM::State::serialize(Writer& writer) const
{
	writer.write("value", m_value);
	writer.write("transitions", m_transitions);
}

void Minty::FSM::State::deserialize(Reader& reader)
{
	reader.read("value", m_value);
	reader.read("transitions", m_transitions);
}

UUID Minty::FSM::create_state(String const& name, Variable const& value)
{
	UUID id = UUID::create();
	m_states.emplace(id, name, State(value));
	return id;
}

inline UUID Minty::FSM::find_state(String const& name) const
{
	if (!m_states.contains(name)) return INVALID_UUID;

	return m_states.get_id(name);
}

Int Minty::FSM::evaluate()
{
	// if no currently selected state, do nothing
	if (m_currentStateId == INVALID_UUID)
	{
		return 1;
	}

	// to check for infinite loops, keep a set of states that have been traversed
	std::unordered_set<UUID> _visitedStates;

	UUID state = m_currentStateId;
	UUID next = state;

	// keep looping until no more transitions have been made
	while (next != INVALID_UUID)
	{
		// move to next state
		state = next;
		next = get_state(next).evaluate(m_scope);

		// if we have visited this state, then an infinite loop has occured
		if (!_visitedStates.emplace(next).second)
		{
			// could not add: already been visited
			return 2;
		}
	}

	// if nothing happened, return -1
	if (state == m_currentStateId)
	{
		return -1;
	}

	// successfully transitioned to the next state
	set_current_state(state);

	return 0;
}

void Minty::FSM::reset()
{
	m_scope.reset();
	set_current_state(m_startingStateId);
}

void Minty::FSM::serialize(Writer& writer) const
{
	writer.push_data_const(this);
	writer.write("scope", m_scope);
	writer.write("states", m_states);
	writer.write("start", m_startingStateId);
	writer.pop_data_const();
}

void Minty::FSM::deserialize(Reader& reader)
{
	reader.push_data(this);

	reader.read("scope", m_scope);

	// read states headers first
	if (reader.indent("states"))
	{
		m_states.clear();

		String name;
		UUID id;
		for (Size i = 0; i < reader.size(); i++)
		{
			reader.read_name(i, name);
			reader.read(i, id);

			// add empty state: just name and ID needed
			m_states.emplace(id, name, State());
		}

		reader.outdent();

		// now read all states and data
		Lookup<State> temp;
		reader.read("states", temp);
		m_states = temp;
	}

	// if got starting state, also set current
	if (reader.read("start", m_startingStateId))
	{
		m_currentStateId = m_startingStateId;
	}

	reader.pop_data();
}