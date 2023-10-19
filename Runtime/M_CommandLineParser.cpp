#include "pch.h"
#include "M_CommandLineParser.h"

#include <iostream>

using namespace minty;

void CommandLineParser::add_parameter(Parameter const param)
{
	switch (param.type)
	{
	case Parameter::Type::POSITIONAL:
		_positionalParams.emplace(param.index, param);
		break;
	case Parameter::Type::FLAG:
		_flagParams.emplace(param.flag, param);
		break;
	}
}

void CommandLineParser::parse(int argc, char const* argv[])
{
	Parameter const* param = nullptr;

	// iterate through all arguments
	for (int i = 0; i < argc; i++)
	{
		// find positional argument or flag argument, flag takes priority
		auto flagFound = _flagParams.find(argv[i]);
		auto posFound = _positionalParams.find(i);
		if (flagFound != _flagParams.end())
		{
			param = &flagFound->second;

			// ignore flag itself
			i++;
		}
		else if (posFound != _positionalParams.end())
		{
			param = &posFound->second;
		}
		else
		{
			// no arg found at this position
			continue;
		}

		Argument arg;

		// grab values, store in argument
		for (int j = 0; j < param->argc && j + i < argc; j++)
		{
			arg.args.push_back(argv[i + j]);
		}

		// add/override to total arguments
		_args[param->name] = arg;
	}
}

bool CommandLineParser::get_argument(std::string const& name, Argument& arg)
{
	// try to find the arg
	auto argFound = _args.find(name);
	if (argFound != _args.end())
	{
		// found arg
		arg = argFound->second;

		return true;
	}

	// did not find arg
	return false;
}

bool CommandLineParser::get_argument(std::string const& name)
{
	return _args.find(name) != _args.end();
}

std::string minty::to_string(CommandLineParser const& value)
{
	return std::format("CommandLineParser()");
}
