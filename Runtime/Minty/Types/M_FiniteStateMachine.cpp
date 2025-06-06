#include "pch.h"
#include "Minty/Types/M_FiniteStateMachine.h"

#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_Writer.h"
#include "Minty/Tools/M_Text.h"

using namespace Minty;

Minty::FSM::Scope::Scope()
	: _ids()
	, _values()
{}

bool Minty::FSM::Scope::contains(String const& key) const
{
	return _ids.contains(key);
}

ID Minty::FSM::Scope::emplace(String const& key, int const value)
{
	// get id
	ID id = static_cast<ID>(_values.size());

	// add to ids
	_ids.emplace(key, id);

	// add to values
	_values.push_back(value);

	return id;
}

int Minty::FSM::Scope::get(String const& key) const
{
	return _values.at(_ids.at(key));
}

int Minty::FSM::Scope::get(ID const key) const
{
	return _values.at(key);
}

ID Minty::FSM::Scope::get_id(String const& key) const
{
	return _ids.at(key);
}

String Minty::FSM::Scope::get_name(ID const key) const
{
	for (auto const& pair : _ids)
	{
		if (pair.second == key)
		{
			return pair.first;
		}
	}

	return "";
}

ID Minty::FSM::Scope::find(String const& key) const
{
	auto found = _ids.find(key);

	if (found != _ids.end())
	{
		return found->second;
	}

	return ERROR_ID;
}

void Minty::FSM::Scope::set(ID const key, int const value)
{
	_values[key] = value;
}

void Minty::FSM::Scope::set(String const& key, int const value)
{
	_values[_ids.at(key)] = value;
}

void Minty::FSM::Scope::clear()
{
	_ids.clear();
	_values.clear();
}

void Minty::FSM::Scope::reset()
{
	for (size_t i = 0; i < _values.size(); i++)
	{
		_values[i] = 0;
	}
}

void Minty::FSM::Scope::serialize(Writer& writer) const
{
	// write each name and value: ids are calculated automatically when deserialized
	for (auto const& pair : _ids)
	{
		writer.write(pair.first, _values.at(pair.second));
	}
}

void Minty::FSM::Scope::deserialize(Reader const& reader)
{
	clear();

	// read all parts of the node as the variables
	Node const& node = reader.get_node();
	for (auto const& child : node.get_children())
	{
		emplace(child.get_name(), child.to_int());
	}
}

Minty::FSM::Condition::Condition()
	: _variableId(ERROR_ID)
	, _conditional()
	, _value()
{}

Minty::FSM::Condition::Condition(String const& value, FSM const& fsm)
	: _variableId(ERROR_ID)
	, _conditional()
	, _value()
{
	// split into parts
	auto list = Text::split(value);
	size_t size = list.size();

	// get ID of variable from fsm
	_variableId = fsm.find_variable(Text::join(list, 0, size - 2));
	_conditional = from_string_conditional(list.at(size - 2));
	_value = Parse::to_int(list.at(size - 1));
}

Minty::FSM::Condition::Condition(ID const variableId, Conditional const conditional, int const value)
	: _variableId(variableId)
	, _conditional(conditional)
	, _value(value)
{}

bool Minty::FSM::Condition::evaluate(Scope const& scope) const
{
	//get value of variable from the scope
	int value = scope.get(_variableId);

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

String Minty::FSM::Condition::to_pretty_string(FSM const& fsm) const
{
	return std::format("{} {} {}", fsm.get_variable_name(_variableId), to_string(_conditional), _value);
}

void Minty::FSM::Condition::serialize(Writer& writer) const
{
	FSM const* fsm = static_cast<FSM const*>(writer.get_data());

	writer.get_node().set_data(to_pretty_string(*fsm));
}

void Minty::FSM::Condition::deserialize(Reader const& reader)
{
	FSM const* fsm = static_cast<FSM const*>(reader.get_data());

	std::vector<String> parts = Text::split(reader.get_node().get_data());

	_variableId = fsm->find_variable(parts.at(0));
	_conditional = from_string_conditional(parts.at(1));
	_value = Parse::to_int(parts.at(2));
}

Minty::FSM::Transition::Transition()
	: _stateId(ERROR_ID)
	, _conditions()
{}

Minty::FSM::Transition::Transition(ID const stateId, std::vector<Condition> const& conditions)
	: _stateId(stateId)
	, _conditions(conditions)
{}

ID Minty::FSM::Transition::get_state() const
{
	return _stateId;
}

void Minty::FSM::Transition::emplace_condition(Condition const& condition)
{
	_conditions.push_back(condition);
}

bool Minty::FSM::Transition::evaluate(Scope const& scope) const
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

void Minty::FSM::Transition::serialize(Writer& writer) const
{
	FSM const* fsm = static_cast<FSM const*>(writer.get_data());

	writer.write("state", fsm->get_state_name(_stateId));

	Node condNode("conditions");
	Writer condWriter(condNode, writer.get_data());

	for (auto const& condition : _conditions)
	{
		condWriter.write("", condition.to_pretty_string(*fsm));
	}

	writer.write(condNode);
}

void Minty::FSM::Transition::deserialize(Reader const& reader)
{
	FSM const* fsm = static_cast<FSM const*>(reader.get_data());

	_stateId = fsm->find_state(reader.read_string("state"));

	Node const& node = reader.get_node();
	if (auto const* conditions = node.find("conditions"))
	{
		std::vector<Node const*> list = conditions->find_all("");
		for (Node const* node : list)
		{
			Reader condReader(*node, reader.get_data());
			Condition cond;
			cond.deserialize(condReader);
			emplace_condition(cond);
		}
	}
}

Minty::FSM::State::State()
	: _value()
	, _transitions()
{}

Minty::FSM::State::State(Dynamic const& value)
	: _value(value)
	, _transitions()
{}

Dynamic& Minty::FSM::State::get_value()
{
	return _value;
}

Dynamic const& Minty::FSM::State::get_value() const
{
	return _value;
}

void Minty::FSM::State::emplace_transition(Transition const& transition)
{
	_transitions.push_back(transition);
}

ID Minty::FSM::State::evaluate(Scope const& scope) const
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

void Minty::FSM::State::serialize(Writer& writer) const
{
	writer.write("value", _value);
	writer.write("transitions", _transitions);
}

void Minty::FSM::State::deserialize(Reader const& reader)
{
	reader.read_object_ref("value", _value);
	reader.read_vector("transitions", _transitions);
}

Minty::FSM::FSM()
	: _scope()
	, _states()
	, _startingStateId(0) // start with the first State created, by default
	, _currentStateId(0)  // "
{}

Minty::FSM::~FSM()
{}

ID Minty::FSM::create_state(String const& name, Dynamic const& value)
{
	return _states.emplace(name, State(value));
}

FSM::State& Minty::FSM::get_state(ID const id)
{
	return _states.at(id);
}

FSM::State const& Minty::FSM::get_state(ID const id) const
{
	return _states.at(id);
}

String Minty::FSM::get_state_name(ID const id) const
{
	return _states.get_name(id);
}

ID Minty::FSM::find_state(String const& name) const
{
	return _states.find(name);
}

void Minty::FSM::set_starting_state(ID const id)
{
	_startingStateId = id;
}

void Minty::FSM::set_starting_state(String const& name)
{
	_startingStateId = _states.get_id(name);
}

void Minty::FSM::set_current_state(ID const id)
{
	_currentStateId = id;
}

void Minty::FSM::set_current_state(String const& name)
{
	_currentStateId = _states.get_id(name);
}

bool Minty::FSM::has_current_state() const
{
	return _currentStateId != ERROR_ID;
}

FSM::State& Minty::FSM::get_current_state()
{
	return _states.at(_currentStateId);
}

FSM::State const& Minty::FSM::get_current_state() const
{
	return _states.at(_currentStateId);
}

Dynamic& Minty::FSM::get_current_value()
{
	return _states.at(_currentStateId).get_value();
}

Dynamic const& Minty::FSM::get_current_value() const
{
	return _states.at(_currentStateId).get_value();
}

ID Minty::FSM::create_variable(String const& name, int const initialValue)
{
	return _scope.emplace(name, initialValue);
}

void Minty::FSM::set_variable(ID const id, int const value)
{
	_scope.set(id, value);
}

void Minty::FSM::set_variable(String const& name, int const value)
{
	_scope.set(name, value);
}

int Minty::FSM::get_variable(ID const id) const
{
	return _scope.get(id);
}

int Minty::FSM::get_variable(String const& name) const
{
	return _scope.get(name);
}

String Minty::FSM::get_variable_name(ID const id) const
{
	return _scope.get_name(id);
}

ID Minty::FSM::find_variable(String const& name) const
{
	return _scope.find(name);
}

int Minty::FSM::evaluate()
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
		if (!_visitedStates.emplace(next).second)
		{
			// could not add: already been visited
			return 2;
		}
	}

	// if nothing happened, return -1
	if (state == _currentStateId)
	{
		return -1;
	}

	// successfully transitioned to the next state
	set_current_state(state);

	return 0;
}

void Minty::FSM::reset()
{
	_scope.reset();
	set_current_state(_startingStateId);
}

void Minty::FSM::serialize(Writer& writer) const
{
	// swap data
	Writer fsmWriter(writer.get_node(), this);

	fsmWriter.write("scope", _scope);
	fsmWriter.write("states", _states);
	fsmWriter.write("start", _states.get_name(_startingStateId));
}

void Minty::FSM::deserialize(Reader const& reader)
{
	Reader fsmReader(reader.get_node(), this);

	fsmReader.read_object_ref("scope", _scope);

	// read all state names first so that the transitions can reference them
	if (Node const* statesNode = fsmReader.get_node().find("states"))
	{
		_states.clear();

		for (Node const& stateNode : statesNode->get_children())
		{
			_states.emplace(stateNode.get_name(), State());
		}

		// read into temp register so the temp states still exist
		Register<State> temp;
		fsmReader.read_register("states", temp);
		_states = temp; // copy over
	}

	_startingStateId = _states.find(fsmReader.read_string("start"));
	_currentStateId = _startingStateId;
}

String Minty::to_string(Conditional const value)
{
	switch (value)
	{
	case Conditional::Equal: return "==";
	case Conditional::NotEqual: return "!=";
	case Conditional::GreaterThan: return ">";
	case Conditional::GreaterThanOrEqualTo: return ">=";
	case Conditional::LessThan: return "<";
	case Conditional::LessThanOrEqualTo: return "<=";
	default: return "";
	}
}

Conditional Minty::from_string_conditional(String const& value)
{
	if (value == "==") return Conditional::Equal;
	if (value == "!=") return Conditional::NotEqual;
	if (value == ">") return Conditional::GreaterThan;
	if (value == ">=") return Conditional::GreaterThanOrEqualTo;
	if (value == "<") return Conditional::LessThan;
	if (value == "<=") return Conditional::LessThanOrEqualTo;

	return static_cast<Conditional>(0);
}
