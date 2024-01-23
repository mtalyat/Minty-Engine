#include "pch.h"
#include "M_FiniteStateMachine.h"

using namespace minty;

minty::FSM::Scope::Scope()
	: _ids()
	, _values()
{}

bool minty::FSM::Scope::contains(String const& key) const
{
	return _ids.contains(key);
}

ID minty::FSM::Scope::emplace(String const& key, int const value)
{
	// get id
	ID id = static_cast<ID>(_values.size());

	// add to ids
	_ids.emplace(key, id);

	// add to values
	_values.push_back(value);

	return id;
}

int minty::FSM::Scope::get(String const& key) const
{
	return _values.at(_ids.at(key));
}

int minty::FSM::Scope::get(ID const key) const
{
	return _values.at(key);
}

ID minty::FSM::Scope::get_id(String const& key) const
{
	return _ids.at(key);
}

String minty::FSM::Scope::get_name(ID const key) const
{
	for (auto const& pair : _ids)
	{
		if (pair.second == key)
		{
			return pair.first;
		}
	}

	return string::EMPTY;
}

ID minty::FSM::Scope::find(String const& key) const
{
	auto const& found = _ids.find(key);

	if (found != _ids.end())
	{
		return found->second;
	}

	return ERROR_ID;
}

void minty::FSM::Scope::set(ID const key, int const value)
{
	_values[key] = value;
}

void minty::FSM::Scope::set(String const& key, int const value)
{
	_values[_ids.at(key)] = value;
}

void minty::FSM::Scope::clear()
{
	_ids.clear();
	_values.clear();
}

void minty::FSM::Scope::reset()
{
	for (size_t i = 0; i < _values.size(); i++)
	{
		_values[i] = 0;
	}
}

void minty::FSM::Scope::serialize(Writer& writer) const
{
	// write each name and value: ids are calculated automatically when deserialized
	for (auto const& pair : _ids)
	{
		writer.write(pair.first, _values.at(pair.second));
	}
}

void minty::FSM::Scope::deserialize(Reader const& reader)
{
	clear();

	// read all parts of the node as the variables
	Node const& node = reader.get_node();
	for (auto const& child : node.get_children())
	{
		emplace(child.get_name(), child.to_int());
	}
}

minty::FSM::Condition::Condition()
	: _variableId(ERROR_ID)
	, _conditional()
	, _value()
{}

minty::FSM::Condition::Condition(String const& value, FSM const& fsm)
	: _variableId(ERROR_ID)
	, _conditional()
	, _value()
{
	// split into parts
	auto list = string::split(value);
	size_t size = list.size();

	// get ID of variable from fsm
	_variableId = fsm.find_variable(string::join(list, 0, size - 2));
	_conditional = from_string_conditional(list.at(size - 2));
	_value = parse::to_int(list.at(size - 1));
}

minty::FSM::Condition::Condition(ID const variableId, Conditional const conditional, int const value)
	: _variableId(variableId)
	, _conditional(conditional)
	, _value(value)
{}

bool minty::FSM::Condition::evaluate(Scope const& scope) const
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

String minty::FSM::Condition::to_pretty_string(FSM const& fsm) const
{
	return std::format("{} {} {}", fsm.get_variable_name(_variableId), to_string(_conditional), _value);
}

void minty::FSM::Condition::serialize(Writer& writer) const
{
	FSM const* fsm = static_cast<FSM const*>(writer.get_data());

	writer.write("variable", fsm->get_variable_name(_variableId));
	writer.write("conditional", to_string(_conditional));
	writer.write("value", _value);
}

void minty::FSM::Condition::deserialize(Reader const& reader)
{
	FSM const* fsm = static_cast<FSM const*>(reader.get_data());

	_variableId = fsm->find_variable(reader.read_string("variable"));
	_conditional = from_string_conditional(reader.read_string("conditional"));
	_value = reader.read_int("value");
}

minty::FSM::Transition::Transition()
	: _stateId(ERROR_ID)
	, _conditions()
{}

minty::FSM::Transition::Transition(ID const stateId, std::vector<Condition> const& conditions)
	: _stateId(stateId)
	, _conditions(conditions)
{}

ID minty::FSM::Transition::get_state() const
{
	return _stateId;
}

void minty::FSM::Transition::emplace_condition(Condition const& condition)
{
	_conditions.push_back(condition);
}

bool minty::FSM::Transition::evaluate(Scope const& scope) const
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

void minty::FSM::Transition::serialize(Writer& writer) const
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

void minty::FSM::Transition::deserialize(Reader const& reader)
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

minty::FSM::State::State()
	: _value()
	, _transitions()
{}

minty::FSM::State::State(Dynamic const& value)
	: _value(value)
	, _transitions()
{}

Dynamic& minty::FSM::State::get_value()
{
	return _value;
}

Dynamic const& minty::FSM::State::get_value() const
{
	return _value;
}

void minty::FSM::State::emplace_transition(Transition const& transition)
{
	_transitions.push_back(transition);
}

ID minty::FSM::State::evaluate(Scope const& scope) const
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

void minty::FSM::State::serialize(Writer& writer) const
{
	writer.write_object("value", _value);
	writer.write("transitions", _transitions);
}

void minty::FSM::State::deserialize(Reader const& reader)
{
	reader.read_object("value", _value);
	reader.read_vector("transitions", _transitions);
}

minty::FSM::FSM()
	: _scope()
	, _states()
	, _startingStateId(0) // start with the first State created, by default
	, _currentStateId(0)  // "
{}

minty::FSM::~FSM()
{}

ID minty::FSM::create_state(String const& name, Dynamic const& value)
{
	return _states.emplace(name, State(value));
}

FSM::State& minty::FSM::get_state(ID const id)
{
	return _states.at(id);
}

FSM::State const& minty::FSM::get_state(ID const id) const
{
	return _states.at(id);
}

String minty::FSM::get_state_name(ID const id) const
{
	return _states.get_name(id);
}

ID minty::FSM::find_state(String const& name) const
{
	return _states.find(name);
}

void minty::FSM::set_starting_state(ID const id)
{
	_startingStateId = id;
}

void minty::FSM::set_starting_state(String const& name)
{
	_startingStateId = _states.get_id(name);
}

void minty::FSM::set_current_state(ID const id)
{
	_currentStateId = id;
}

void minty::FSM::set_current_state(String const& name)
{
	_currentStateId = _states.get_id(name);
}

FSM::State& minty::FSM::get_current_state()
{
	return _states.at(_currentStateId);
}

FSM::State const& minty::FSM::get_current_state() const
{
	return _states.at(_currentStateId);
}

Dynamic& minty::FSM::get_current_value()
{
	return _states.at(_currentStateId).get_value();
}

Dynamic const& minty::FSM::get_current_value() const
{
	return _states.at(_currentStateId).get_value();
}

ID minty::FSM::create_variable(String const& name, int const initialValue)
{
	return _scope.emplace(name, initialValue);
}

void minty::FSM::set_variable(ID const id, int const value)
{
	_scope.set(id, value);
}

void minty::FSM::set_variable(String const& name, int const value)
{
	_scope.set(name, value);
}

int minty::FSM::get_variable(ID const id) const
{
	return _scope.get(id);
}

int minty::FSM::get_variable(String const& name) const
{
	return _scope.get(name);
}

String minty::FSM::get_variable_name(ID const id) const
{
	return _scope.get_name(id);
}

ID minty::FSM::find_variable(String const& name) const
{
	return _scope.find(name);
}

int minty::FSM::evaluate()
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

void minty::FSM::reset()
{
	_scope.reset();
	set_current_state(_startingStateId);
}

void minty::FSM::serialize(Writer& writer) const
{
	// swap data
	Writer fsmWriter(writer.get_node(), this);

	fsmWriter.write_object("scope", _scope);
	fsmWriter.write("states", _states);
	fsmWriter.write("start", _states.get_name(_startingStateId));
}

void minty::FSM::deserialize(Reader const& reader)
{
	Reader fsmReader(reader.get_node(), this);

	fsmReader.read_object("scope", _scope);
	fsmReader.read_register("states", _states);
	_startingStateId = _states.find(fsmReader.read_string("start"));
	_currentStateId = _startingStateId;
}

String minty::to_string(Conditional const value)
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

Conditional minty::from_string_conditional(String const& value)
{
	if (value == "==") return Conditional::Equal;
	if (value == "!=") return Conditional::NotEqual;
	if (value == ">") return Conditional::GreaterThan;
	if (value == ">=") return Conditional::GreaterThanOrEqualTo;
	if (value == "<") return Conditional::LessThan;
	if (value == "<=") return Conditional::LessThanOrEqualTo;

	return static_cast<Conditional>(0);
}
