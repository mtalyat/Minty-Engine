#include "pch.h"
#include "Minty/Types/M_CommandLineParser.h"

#include <iostream>

using namespace Minty;

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
		char const* argument = argv[i];
		auto flagFound = _flagParams.find(argument);
		auto posFound = _positionalParams.find(i);

		Argument arg;

		if (flagFound != _flagParams.end())
		{
			param = &flagFound->second;

			// grab values, store in argument
			for (int j = 1; j <= param->argc && j + i <= argc; j++)
			{
				arg.args.push_back(argv[i + j]);
			}
		}
		else if (posFound != _positionalParams.end())
		{
			param = &posFound->second;

			// grab values, store in argument
			for (int j = 0; j < param->argc && j + i < argc; j++)
			{
				arg.args.push_back(argv[i + j]);
			}
		}
		else
		{
			// no arg found at this position
			continue;
		}

		// add/override to total arguments
		_args[param->name] = arg;
	}
}

bool CommandLineParser::get_argument(String const& name, Argument& arg)
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

bool CommandLineParser::get_argument(String const& name)
{
	return _args.find(name) != _args.end();
}

String Minty::to_string(CommandLineParser const& value)
{
	return std::format("CommandLineParser()");
}
