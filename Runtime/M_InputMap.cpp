#include "pch.h"
#include "M_InputMap.h"

using namespace minty;

minty::InputMap::InputMap()
	: _keyDownEvents(new std::map<Key, KeyEvent_t>())
	, _keyUpEvents(new std::map<Key, KeyEvent_t>())
	, _keyEvents(new std::map<Key, KeyEvent_t>())
	, _mouseDownEvents(new std::map<MouseButton, ClickEvent_t>())
	, _mouseUpEvents(new std::map<MouseButton, ClickEvent_t>())
	, _mouseEvents(new std::map<MouseButton, ClickEvent_t>())
	, _mouseMoveEvent(new MoveEvent_t())
	, _mouseScrollEvent(new ScrollEvent_t())
{}

minty::InputMap::~InputMap()
{
	delete _keyDownEvents;
	delete _keyUpEvents;
	delete _keyEvents;
	delete _mouseDownEvents;
	delete _mouseUpEvents;
	delete _mouseEvents;
	delete _mouseMoveEvent;
	delete _mouseScrollEvent;
}

void minty::InputMap::emplace_key_down(Key const key, KeyEvent_t::func const& func)
{
	(*_keyDownEvents)[key].emplace(func);
}

void minty::InputMap::emplace_key_up(Key const key, KeyEvent_t::func const& func)
{
	(*_keyUpEvents)[key].emplace(func);
}

void minty::InputMap::emplace_key(Key const key, KeyEvent_t::func const& func)
{
	(*_keyEvents)[key].emplace(func);
}

void minty::InputMap::emplace_mouse_down(MouseButton const button, ClickEvent_t::func const& func)
{
	(*_mouseDownEvents)[button].emplace(func);
}

void minty::InputMap::emplace_mouse_up(MouseButton const button, ClickEvent_t::func const& func)
{
	(*_mouseUpEvents)[button].emplace(func);
}

void minty::InputMap::emplace_mouse(MouseButton const button, ClickEvent_t::func const& func)
{
	(*_mouseEvents)[button].emplace(func);
}

void minty::InputMap::emplace_mouse_move(MoveEvent_t::func const& func)
{
	_mouseMoveEvent->emplace(func);
}

void minty::InputMap::emplace_mouse_scroll(ScrollEvent_t::func const& func)
{
	_mouseScrollEvent->emplace(func);
}

void minty::InputMap::erase_key_down(Key const key, KeyEvent_t::func const& func)
{
	auto found = _keyDownEvents->find(key);
	if (found != _keyDownEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_key_up(Key const key, KeyEvent_t::func const& func)
{
	auto found = _keyUpEvents->find(key);
	if (found != _keyUpEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_key(Key const key, KeyEvent_t::func const& func)
{
	auto found = _keyEvents->find(key);
	if (found != _keyEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_mouse_down(MouseButton const button, ClickEvent_t::func const& func)
{
	auto found = _mouseDownEvents->find(button);
	if (found != _mouseDownEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_mouse_up(MouseButton const button, ClickEvent_t::func const& func)
{
	auto found = _mouseUpEvents->find(button);
	if (found != _mouseUpEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_mouse(MouseButton const button, ClickEvent_t::func const& func)
{
	auto found = _mouseEvents->find(button);
	if (found != _mouseEvents->end())
	{
		found->second.erase(func);
	}
}

void minty::InputMap::erase_mouse_move(MoveEvent_t::func const& func)
{
	_mouseMoveEvent->erase(func);
}

void minty::InputMap::erase_mouse_scroll(ScrollEvent_t::func const& func)
{
	_mouseScrollEvent->erase(func);
}

void minty::InputMap::invoke_key(KeyPressEventArgs const& args) const
{
	switch (args.action)
	{
	case KeyAction::Up:
	{
		auto found = _keyUpEvents->find(args.key);
		if (found != _keyUpEvents->end())
		{
			found->second.invoke(args);
		}
	}
	break;
	case KeyAction::Down:
	{
		auto found = _keyDownEvents->find(args.key);
		if (found != _keyDownEvents->end())
		{
			found->second.invoke(args);
		}
	}
	break;
	}

	// always do normal event
	auto found = _keyEvents->find(args.key);
	if (found != _keyEvents->end())
	{
		found->second.invoke(args);
	}
}

void minty::InputMap::invoke_mouse_click(MouseClickEventArgs const& args) const
{
	switch (args.action)
	{
	case KeyAction::Up:
	{
		auto found = _mouseUpEvents->find(args.button);
		if (found != _mouseUpEvents->end())
		{
			found->second.invoke(args);
		}
	}
	break;
	case KeyAction::Down:
	{
		auto found = _mouseDownEvents->find(args.button);
		if (found != _mouseDownEvents->end())
		{
			found->second.invoke(args);
		}
	}
	break;
	}

	// always do normal event
	auto found = _mouseEvents->find(args.button);
	if (found != _mouseEvents->end())
	{
		found->second.invoke(args);
	}
}

void minty::InputMap::invoke_mouse_move(MouseMoveEventArgs const& args) const
{
	_mouseMoveEvent->invoke(args);
}

void minty::InputMap::invoke_mouse_scroll(MouseScrollEventArgs const& args) const
{
	_mouseScrollEvent->invoke(args);
}
