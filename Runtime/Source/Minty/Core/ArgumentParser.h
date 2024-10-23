#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/Type.h"
#include <vector>
#include <unordered_map>

namespace Minty
{
	/// <summary>
	/// Handles parsing the command line.
	/// </summary>
	class ArgumentParser
	{
	public:
		/// <summary>
		/// Defines a command line parameter.
		/// </summary>
		struct Parameter
		{
			/// <summary>
			/// The type of parameter.
			/// </summary>
			enum Type
			{
				/// <summary>
				/// The parameter is identified by its position in the order of the parameters.
				/// </summary>
				POSITIONAL,

				/// <summary>
				/// The parameter is identified by a preceeding flag, such as "-f".
				/// </summary>
				FLAG,
			};

			Type const type;
			String const name;
			String const flag;
			Int const index;
			Int const argc;

			/// <summary>
			/// Creates a new positional parameter.
			/// </summary>
			/// <param name="name">The name of the parameter, to get the argument values later.</param>
			/// <param name="index">The index of the parameter, including the name of the program.</param>
			/// <param name="argc">The maximum number of string arguments to capture.</param>
			Parameter(String const& name, Int const index, Int const argc = 1)
				: type(Type::POSITIONAL)
				, name(name)
				, flag("")
				, index(index)
				, argc(argc)
			{}

			/// <summary>
			/// Creates a new flag parameter.
			/// </summary>
			/// <param name="name">The name of the parameter, to get the argument values later.</param>
			/// <param name="flag">The index of the parameter, including the name of the program.</param>
			/// <param name="argc">The maximum number of string arguments to capture.</param>
			Parameter(String const& name, String const& flag, Int const argc = 1)
				: type(Type::FLAG)
				, name(name)
				, flag(flag)
				, index(-1)
				, argc(argc)
			{}
		};

		/// <summary>
		/// Defines an command line argument given in place of a command line parameter.
		/// </summary>
		struct Argument
		{
			std::vector<String> args;
		};

	private:
		std::unordered_map<Int, Parameter> _positionalParams;
		std::unordered_map<String, Parameter> _flagParams;
		std::unordered_map<String, Argument> _args;

	public:
		/// <summary>
		/// Adds the parameter to the list of parameters to parse for.
		/// </summary>
		/// <param name="param"></param>
		void add_parameter(Parameter const param);

		/// <summary>
		/// Parses the loaded parameters.
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void parse(Int argc, char const* argv[]);

		/// <summary>
		/// Gets the argument by name, if the argument was provided.
		/// </summary>
		/// <param name="name">The name of the parameter.</param>
		/// <returns>True if the argument was found.</returns>
		Bool get_argument(String const& name, Argument& arg);

		/// <summary>
		/// Checks if the argument was provided, using the given parameter name.
		/// </summary>
		/// <param name="name">The name of the parameter to check if it exists.</param>
		/// <returns>True if the argument was given.</returns>
		Bool get_argument(String const& name);
	};
}

