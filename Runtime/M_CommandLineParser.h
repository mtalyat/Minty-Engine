#pragma once

#include <string>
#include <vector>
#include <map>

namespace minty
{
	class CommandLineParser
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
			std::string const name;
			std::string const flag;
			int const index;
			int const argc;

			/// <summary>
			/// Creates a new positional parameter.
			/// </summary>
			/// <param name="name">The name of the parameter, to get the argument values later.</param>
			/// <param name="index">The index of the parameter, including the name of the program.</param>
			/// <param name="argc">The maximum number of string arguments to capture.</param>
			Parameter(std::string const& name, int const index, int const argc = 1)
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
			Parameter(std::string const& name, std::string const& flag, int const argc = 1)
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
			std::vector<std::string> args;
		};

	private:
		std::map<int, Parameter> _positionalParams;
		std::map<std::string, Parameter> _flagParams;
		std::map<std::string, Argument> _args;

	public:
		/// <summary>
		/// Adds the parameter to the list of parameters to parse for.
		/// </summary>
		/// <param name="param"></param>
		void addParameter(Parameter const param);

		/// <summary>
		/// Parses the loaded parameters.
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void parse(int argc, char const* argv[]);

		/// <summary>
		/// Gets the argument by name, if the argument was provided.
		/// </summary>
		/// <param name="name">The name of the parameter.</param>
		/// <returns>True if the argument was found.</returns>
		bool getArgument(std::string const& name, Argument& arg);

		/// <summary>
		/// Checks if the argument was provided, using the given parameter name.
		/// </summary>
		/// <param name="name">The name of the parameter to check if it exists.</param>
		/// <returns>True if the argument was given.</returns>
		bool getArgument(std::string const& name);
	};
}

